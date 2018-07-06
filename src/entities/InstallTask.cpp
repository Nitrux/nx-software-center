//
// Created by alexis on 7/4/18.
//

#include <iostream>
#include <appimage/appimage.h>
#include <gateways/GetApplicationRequest.h>
#include <gateways/FileDownload.h>
#include "InstallTask.h"
void InstallTask::setRestClient(RestClient* restClient)
{
    InstallTask::restClient = restClient;
}
void InstallTask::setId(const QString& id)
{
    InstallTask::id = id;
}
InstallTask::InstallTask()
        :QObject(), restClient(nullptr), running(false), getApplicationRequest(nullptr), fileDownload(nullptr) { }
void InstallTask::start()
{
    if (running)
        throw std::runtime_error("InstallTask already running");

    running = true;
    getApplicationInfo();
}
void InstallTask::getApplicationInfo()
{
    if (restClient) {
        getApplicationRequest = restClient->buildGetApplicationRequest(id);
        connect(getApplicationRequest, &GetApplicationRequest::resultReady, this,
                &InstallTask::handleGetApplicationInfoCompleted);
        connect(getApplicationRequest, &GetApplicationRequest::failed, this,
                &InstallTask::handleGetApplicationInfoFailed);

        getApplicationRequest->start();
    }
    else
        throw std::runtime_error("Uninitialized InstallTask: Missing RestClient.");
}
void InstallTask::handleGetApplicationInfoCompleted()
{
    auto applicationInfo = getApplicationRequest->getResult();

    if (!applicationInfo.releases.isEmpty())
        downloadApplicationFile(applicationInfo);
    else emit failed("No releases available");

    getApplicationRequest->deleteLater();
    getApplicationRequest = nullptr;
}
void InstallTask::downloadApplicationFile(ApplicationFull& applicationInfo)
{
    ApplicationFull::Release r = applicationInfo.latestCompatibleRelease(QSysInfo::currentCpuArchitecture());

    const auto files = r.compatibleFiles(QSysInfo::currentCpuArchitecture());

    if (files.isEmpty())
            emit failed("No files compatible with this architecture");
    else {
        const auto& file = files.first();

        QString filePath = getDownloadFilePath(applicationInfo, r, file);

        fileDownload = new FileDownload(file.url, filePath);
        fileDownload->setNetworkAccessManager(restClient->getNetworkAccessManager());
        connect(fileDownload, &FileDownload::completed, this, &InstallTask::handleFileDownloadCompleted);
        fileDownload->start();
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
    getApplicationRequest->deleteLater();
    getApplicationRequest = nullptr;

    running = false;
    emit failed("Unable to access application information: "+reason);
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
    const auto filePath = fileDownload->getTarget_path();
    fileDownload->deleteLater();
    fileDownload = nullptr;

    if (QFile::exists(filePath))
        registerAppImage(filePath);
    else
            emit failed("Download failed");
}
void InstallTask::registerAppImage(const QString& filePath)
{
    appimage_register_in_system(filePath.toStdString().c_str(), false);
    if (appimage_is_registered_in_system((filePath.toStdString().c_str())))
            emit completed();
    else
            emit failed("Unable to register in system");
}
