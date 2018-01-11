//
// Created by alexis on 10/01/18.
//

#include <QDir>
#include <QThread>
#include <QDebug>

#include <entities/Application.h>
#include <gateways/DownloadManager.h>
#include "InstallAppImageInteractor.h"


InstallAppImageInteractor::InstallAppImageInteractor(const Application &application, DownloadManager *downloadManager,
                                                     QObject *parent) : Interactor(parent), app(application),
                                                                        downloadManager(downloadManager),
downloadJob(nullptr) {
    QString appFileName = QString("%1-%2_%3.AppImage").arg(app.getCodeName(),
                                                        app.getVersion(),
                                                           app.getArch());
    createInstallationDirIfNotExist();
    createInstallationPath(appFileName);

    QString d = QString("Installing %1 %2").arg(app.getName(), app.getVersion());

    metadata.insert(Interactor::META_KEY_DESCRIPTION, d);
    metadata.insert(Interactor::META_KEY_STATUS, Interactor::STATUS_CREATED);
    metadata.insert(Interactor::META_KEY_TYPE, "install");
}

void InstallAppImageInteractor::execute() {
    setRunningMetadata();


    QDir home = QDir::home();
    home.mkdir("bin");

    downloadJob = downloadManager->downloadToFile(app.getDownloadUrl(),
                                                  installationPath);

    connect(downloadJob, &DownloadToFileJob::progress, this,
            &InstallAppImageInteractor::handleDownloadJobProgress);
    connect(downloadJob, &DownloadToFileJob::finished, this,
            &InstallAppImageInteractor::handleDownloadJobFinished);
    connect(downloadJob, &DownloadToFileJob::error, this,
            &InstallAppImageInteractor::handleDownloadJobError);
}

void InstallAppImageInteractor::setCompletedMetadata() {
    QVariantMap statusUpdate;
    statusUpdate.insert(META_KEY_STATUS, Interactor::STATUS_COMPLETED);
    setMetadata(statusUpdate);
}

void InstallAppImageInteractor::handleDownloadJobProgress(const int value, const int total, const QString &message)
{
    QVariantMap statusUpdate;
    statusUpdate.insert(META_KEY_PROGRESS_MESSAGE, message);
    statusUpdate.insert(META_KEY_PROGRESS_VALUE, value);
    statusUpdate.insert(META_KEY_PROGRESS_TOTAL, total);

    setMetadata(statusUpdate);
}

void InstallAppImageInteractor::handleDownloadJobFinished()
{
    QFile f(installationPath);
    if (f.exists()) {
        f.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner);
    } else {
        qCritical() << "Target file not exist: " << installationPath;
    }

    setCompletedMetadata();
    emit completed();
}

void InstallAppImageInteractor::handleDownloadJobError(const QString &error)
{
    qWarning() << "Download Error: " << app.getDownloadUrl() << " " << error;
    emit completed();
}

void InstallAppImageInteractor::setRunningMetadata() {
    QVariantMap statusUpdate;
    statusUpdate.insert(META_KEY_STATUS, Interactor::STATUS_RUNNING);
    setMetadata(statusUpdate);
}

void InstallAppImageInteractor::createInstallationDirIfNotExist()
{
    QDir dir = QDir::home();
    dir.mkdir("bin");
}

void InstallAppImageInteractor::createInstallationPath(QString appFileName)
{
    QDir dir = QDir::home();
    dir.cd("bin");
    installationPath =  dir.filePath(appFileName);
}
