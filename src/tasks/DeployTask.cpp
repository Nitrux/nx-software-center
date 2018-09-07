//
// Created by alexis on 7/4/18.
//

#include <QtCore/QFileInfo>
#include <QtConcurrent/QtConcurrent>
#include <gateways/ApplicationGetRequest.h>
#include <gateways/FileDownload.h>
#include <gateways/ApplicationRepository.h>
#include <appimage/appimage.h>
#include <tasks/TaskMetadata.h>
#include "DeployTask.h"

void DeployTask::setRepository(ApplicationRepository *restClient) {
    DeployTask::repository = restClient;
}

void DeployTask::setId(const QString &id) {
    DeployTask::id = id;
}

DeployTask::DeployTask()
        : Task(), repository(nullptr), running(false), applicationRepositoryGet(nullptr), fileDownload(nullptr) {
    type = TaskMetadata::VALUE_TYPE_DELPOY;
}

void DeployTask::start() {
    if (running)
        throw std::runtime_error("InstallTask already running");

    running = true;
    setStatus(VALUE_STATUS_RUNNING);
    appImageInfo.id = id;
    getApplicationInfo();
}

void DeployTask::getApplicationInfo() {
    if (repository) {
        setProgressMessage("Getting download information.");
        applicationRepositoryGet = repository->buildGetApplicationRequest(id);
        connect(applicationRepositoryGet, &ApplicationGetRequest::completed, this,
                &DeployTask::handleGetApplicationInfoCompleted);
        connect(applicationRepositoryGet, &ApplicationGetRequest::failed, this,
                &DeployTask::handleGetApplicationInfoFailed);

        applicationRepositoryGet->start();
    } else
        throw std::runtime_error("Uninitialized InstallTask: Missing ApplicationRepositoryRestClient.");
}

void DeployTask::handleGetApplicationInfoCompleted() {
    auto application = applicationRepositoryGet->getApplication();
    appImageInfo = application.latestCompatibleReleaseInfo();
    emit changed();

    if (!appImageInfo.file.url.isEmpty())
        downloadApplicationFile(appImageInfo);
    else
        fail("No releases available");

    applicationRepositoryGet->deleteLater();
    applicationRepositoryGet = nullptr;
}

void DeployTask::fail(const QString &msg) {
    setStatus(VALUE_STATUS_FAILED);
    setProgressMessage(msg);

    emit failed(msg);
}

void DeployTask::downloadApplicationFile(AppImageInfo applicationInfo) {
    const auto files = applicationInfo.release.files;
    if (files.isEmpty()) {
        fail("No files compatible with this architecture");
    } else {
        const auto &file = files.first();

        QString filePath = getDownloadFilePath(applicationInfo, applicationInfo.release, file);

        fileDownload = repository->buildFileDownloadRequest(file.url, filePath);
        connect(fileDownload, &FileDownload::completed, this, &DeployTask::handleFileDownloadCompleted);
        connect(fileDownload, &FileDownload::stopped, this, &DeployTask::handleFileDownloadStopped);
        connect(fileDownload, &FileDownload::progress, this, &DeployTask::handleFileDownloadProgress);
        fileDownload->start();

        setProgressMessage("Downloading AppImage file.");
    }
}

QString DeployTask::getDownloadFilePath(AppImageInfo applicationInfo, const Application::Release &r,
                                        const Application::File &file) const {
    QStringList fileBaseName{applicationInfo.id, QString::number(r.date.toMSecsSinceEpoch()),
                             r.version, file.architecture};
    QString fileName = fileBaseName.join("-") + ".AppImage";
    QString fullPath = applicationsDir + "/" + fileName;
    return fullPath;
}

void DeployTask::handleGetApplicationInfoFailed(const QString &reason) {
    applicationRepositoryGet->deleteLater();
    applicationRepositoryGet = nullptr;

    running = false;
    fail("Unable to access application information: " + reason);
}

const QString &DeployTask::getChannel() const {
    return channel;
}

void DeployTask::setChannel(const QString &channel) {
    DeployTask::channel = channel;
}

void DeployTask::setApplicationsDir(const QString &applicationsDir) {
    DeployTask::applicationsDir = applicationsDir;
}

void DeployTask::handleFileDownloadCompleted() {
    auto filePath = fileDownload->getTargetPath();
    QFileInfo f(filePath);
    filePath = f.absoluteFilePath();
    fileDownload->deleteLater();
    fileDownload = nullptr;

    if (!QFile::exists(filePath))
        fail("FileDownload failed");

    appImageInfo.file.path = filePath;
    verifyAppImageFile(filePath);
}

void DeployTask::registerAppImage(const QString &filePath) {
    setProgressMessage("Registering in the system.");
    appimage_register_in_system(filePath.toStdString().c_str(), false);

    Q_ASSERT(deployedApplicationsRegistry);
    deployedApplicationsRegistry->registerFile(filePath, appImageInfo);
    if (appimage_is_registered_in_system((filePath.toStdString().c_str())))
        complete();
    else
        fail("Unable to register in system");
}

void DeployTask::complete() {
    setStatus(VALUE_STATUS_COMPLETED);
    setProgressMessage("Completed successfully");emit
    completed();
}

void DeployTask::stop() {
    if (applicationRepositoryGet != nullptr) {
        applicationRepositoryGet->stop();
    }

    if (fileDownload != nullptr) {
        fileDownload->stop();
    }
}

void DeployTask::handleFileDownloadProgress(qint64 progress, qint64 total, const QString &message) {
    setProgressValue(progress);
    setProgressTotal(total);
    setProgressMessage(message);
}

void DeployTask::handleFileDownloadStopped() {
    auto filePath = fileDownload->getTargetPath();
    QFile::remove(filePath);

    fileDownload->deleteLater();
    fileDownload = nullptr;

    fail("FileDownload stopped");
}

void DeployTask::setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry) {
    DeployTask::deployedApplicationsRegistry = deployedApplicationsRegistry;
}

void DeployTask::verifyAppImageFile(QString path) {
    setProgressMessage("Verifying AppImage File");

    auto appImageCheckSumFuture = QtConcurrent::run([=]() {
        auto process = new QProcess();
        QStringList arguments{path};
        process->start("sha512sum", arguments);

        process->waitForFinished();
        if (process->exitCode() != 0)
            throw std::runtime_error(process->errorString().toStdString());
        QString rawOutput = process->readAll();
        QString sha512checksum = rawOutput.section(" ", 0, 0).trimmed();

        process->deleteLater();
        return sha512checksum;
    });

    appImageCheckSumFutureWatcher.setFuture(appImageCheckSumFuture);
    connect(&appImageCheckSumFutureWatcher, &QFutureWatcher<QString>::finished, this,
            &DeployTask::handleAppImageCheckSumFutureFinished);

}

void DeployTask::handleAppImageCheckSumFutureFinished() {
    if (appImageInfo.file.sha512checksum != appImageCheckSumFutureWatcher.result()) {
        QFile::remove(appImageInfo.file.path);
        fail("Check sum verification failed.");
    } else {
        registerAppImage(appImageInfo.file.path);
        QFile::setPermissions(appImageInfo.file.path, QFile::ReadOwner | QFile::ExeOwner);
    }
}
