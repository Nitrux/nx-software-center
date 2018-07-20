//
// Created by alexis on 7/4/18.
//

#include <iostream>
#include <appimage/appimage.h>
#include <gateways/ApplicationGetRequest.h>
#include <gateways/FileDownload.h>
#include <QtCore/QFileInfo>
#include <gateways/ApplicationRepository.h>
#include "InstallTask.h"
void InstallTask::setRepository(ApplicationRepository *restClient)
{
    InstallTask::repository = restClient;
}
void InstallTask::setId(const QString& id)
{
    InstallTask::id = id;
}
InstallTask::InstallTask()
        :Task(), repository(nullptr), running(false), applicationRepositoryGet(nullptr), fileDownload(nullptr) { }
void InstallTask::start()
{
    if (running)
        throw std::runtime_error("InstallTask already running");

    running = true;
    setStatus(VALUE_STATUS_RUNNING);
    getApplicationInfo();
}
void InstallTask::getApplicationInfo()
{
    if (repository) {
        setProgressMessage("Getting download information.");
        applicationRepositoryGet = repository->buildGetApplicationRequest(id);
        connect(applicationRepositoryGet, &ApplicationGetRequest::completed, this,
                &InstallTask::handleGetApplicationInfoCompleted);
        connect(applicationRepositoryGet, &ApplicationGetRequest::failed, this,
                &InstallTask::handleGetApplicationInfoFailed);

        applicationRepositoryGet->start();
    }
    else
        throw std::runtime_error("Uninitialized InstallTask: Missing ApplicationRepositoryRestClient.");
}
void InstallTask::handleGetApplicationInfoCompleted()
{
    application = applicationRepositoryGet->getApplication();
    emit changed();

    if (!application.releases.isEmpty())
        downloadApplicationFile(application);
    else
        fail("No releases available");

    applicationRepositoryGet->deleteLater();
    applicationRepositoryGet = nullptr;
}

void InstallTask::fail(const QString &msg) {
    setStatus(VALUE_STATUS_FAILED);
    setProgressMessage(msg);

    emit failed(msg);
}

void InstallTask::downloadApplicationFile(ApplicationFull& applicationInfo)
{
    ApplicationFull::Release r = applicationInfo.latestCompatibleRelease(QSysInfo::currentCpuArchitecture());

    const auto files = r.compatibleFiles(QSysInfo::currentCpuArchitecture());

    if (files.isEmpty()) {
        fail("No files compatible with this architecture");
    }
    else {
        const auto& file = files.first();

        QString filePath = getDownloadFilePath(applicationInfo, r, file);

        fileDownload = repository->buildFileDownloadRequest(file.url, filePath);
        connect(fileDownload, &FileDownload::completed, this, &InstallTask::handleFileDownloadCompleted);
        connect(fileDownload, &FileDownload::stopped, this, &InstallTask::handleFileDownloadStopped);
        connect(fileDownload, &FileDownload::progress, this, &InstallTask::handleFileDownloadProgress);
        fileDownload->start();

        setProgressMessage("Downloading AppImage file.");
    }
}
QString InstallTask::getDownloadFilePath(const ApplicationFull& applicationInfo, const ApplicationFull::Release& r,
        const ApplicationFull::File& file) const
{
    QStringList fileBaseName{applicationInfo.id, QString::number(r.date.toMSecsSinceEpoch()),
                             r.version, file.architecture};
    QString fileName = fileBaseName.join("-")+".AppImage";
    QString fullPath = applicationsDir+"/"+fileName;
    return fullPath;
}
void InstallTask::handleGetApplicationInfoFailed(const QString& reason)
{
    applicationRepositoryGet->deleteLater();
    applicationRepositoryGet = nullptr;

    running = false;
    fail("Unable to access application information: "+reason);
}
const QString& InstallTask::getChannel() const
{
    return channel;
}
void InstallTask::setChannel(const QString& channel)
{
    InstallTask::channel = channel;
}
void InstallTask::setApplicationsDir(const QString& applicationsDir)
{
    InstallTask::applicationsDir = applicationsDir;
}
void InstallTask::handleFileDownloadCompleted()
{
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
void InstallTask::registerAppImage(const QString& filePath)
{
    setProgressMessage("Registering in the system.");
    appimage_register_in_system(filePath.toStdString().c_str(), false);
    if (appimage_is_registered_in_system((filePath.toStdString().c_str()))) {
        setStatus(VALUE_STATUS_COMPLETED);
        setProgressMessage("Completed successfully");
        emit completed();
    }
    else
            fail("Unable to register in system");
}
void InstallTask::stop()
{
    if (applicationRepositoryGet != nullptr) {
        applicationRepositoryGet->stop();
    }

    if (fileDownload != nullptr) {
        fileDownload->stop();
    }
}

void InstallTask::handleFileDownloadProgress(qint64 progress, qint64 total, const QString &message) {
    setProgressValue(progress);
    setProgressTotal(total);
    setProgressMessage(message);
}

void InstallTask::handleFileDownloadStopped() {
    auto filePath = fileDownload->getTarget_path();
    QFile::remove(filePath);

    fileDownload->deleteLater();
    fileDownload = nullptr;

    fail("Download stopped");
}
