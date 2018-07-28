//
// Created by alexis on 7/4/18.
//

#include <iostream>
#include <appimage/appimage.h>
#include <gateways/ApplicationGetRequest.h>
#include <gateways/FileDownload.h>
#include <QtCore/QFileInfo>
#include <gateways/ApplicationRepository.h>
#include <interactors/TaskMetadata.h>
#include "DeployTask.h"

void DeployTask::setRepository(ApplicationRepository *restClient) {
    DeployTask::repository = restClient;
}

void DeployTask::setId(const QString &id) {
    DeployTask::id = id;
}

DeployTask::DeployTask()
        : Task(), repository(nullptr), running(false), applicationRepositoryGet(nullptr), fileDownload(nullptr) {
    type = TaskMetadata::VALUE_TYPE_INSTALL;
}

void DeployTask::start() {
    if (running)
        throw std::runtime_error("InstallTask already running");

    running = true;
    setStatus(VALUE_STATUS_RUNNING);
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

QString DeployTask::getDownloadFilePath(AppImageInfo applicationInfo, const ApplicationFull::Release &r,
                                        const ApplicationFull::File &file) const {
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
    auto filePath = fileDownload->getTarget_path();
    QFileInfo f(filePath);
    filePath = f.absoluteFilePath();
    fileDownload->deleteLater();
    fileDownload = nullptr;

    if (QFile::exists(filePath))
        registerAppImage(filePath);
    else
        fail("Download failed");
}

void DeployTask::registerAppImage(const QString &filePath) {
    setProgressMessage("Registering in the system.");
    appimage_register_in_system(filePath.toStdString().c_str(), false);
    if (appimage_is_registered_in_system((filePath.toStdString().c_str()))) {
        setStatus(VALUE_STATUS_COMPLETED);
        setProgressMessage("Completed successfully");
        emit completed();
    } else
        fail("Unable to register in system");
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
    auto filePath = fileDownload->getTarget_path();
    QFile::remove(filePath);

    fileDownload->deleteLater();
    fileDownload = nullptr;

    fail("Download stopped");
}
