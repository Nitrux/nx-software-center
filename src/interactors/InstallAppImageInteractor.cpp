//
// Created by alexis on 10/01/18.
//

#include <QDir>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

#include <entities/Application.h>
#include <gateways/DownloadManager.h>
#include "InstallAppImageInteractor.h"
#include "TaskMetadata.h"

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

    metadata.insert(TaskMetadata::KEY_DESCRIPTION, d);
    metadata.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_CREATED);

    metadata.insert(TaskMetadata::KEY_APP_ID, app.getId());
    metadata.insert(TaskMetadata::KEY_APP_CODENAME, app.getCodeName());
    metadata.insert(TaskMetadata::KEY_APP_VERSION, app.getVersion());
    metadata.insert(TaskMetadata::KEY_APP_NAME, app.getName());
    metadata.insert(TaskMetadata::KEY_APP_AUTHOR, app.getAuthors().join(", "));

    metadata.insert(TaskMetadata::KEY_TYPE, TaskMetadata::VALUE_TYPE_INSTALL);
    metadata.insert(TaskMetadata::KEY_DOWNLOAD_URL, app.getDownloadUrl());
    metadata.insert(TaskMetadata::KEY_INSTALLATION_PATH, installationPath);
}

void InstallAppImageInteractor::execute() {
    setRunningMetadata();

    QDir home = QDir::home();
    home.mkdir("bin");

    downloadJob = downloadManager->download(app.getDownloadUrl(),
                                            installationPath);

    connect(downloadJob, &Download::progress,
            this, &InstallAppImageInteractor::handleDownloadJobProgress);
    connect(downloadJob, &Download::completed,
            this, &InstallAppImageInteractor::handleDownloadJobFinished);
    connect(downloadJob, &Download::stopped,
            this, &InstallAppImageInteractor::handleDownloadJobError);
    connect(this, &InstallAppImageInteractor::isCanceledChanged,
            this, &InstallAppImageInteractor::handleCanceled);

    isRunning = true;
    downloadJob->setProgressNotificationsEnabled(true);
    downloadJob->start();

    while (isRunning) {
        qApp->processEvents();
        QThread::usleep(2);
    }
}

void InstallAppImageInteractor::setCompletedMetadata() {
    QVariantMap statusUpdate;

    QString d = QString("Download %1 %2 completed").arg(app.getName(), app.getVersion());
    metadata.insert(TaskMetadata::KEY_DESCRIPTION, d);

    statusUpdate.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_COMPLETED);
    setMetadata(statusUpdate);
}

void InstallAppImageInteractor::handleDownloadJobProgress(const int value, const int total, const QString &message) {
    QVariantMap statusUpdate;
    statusUpdate.insert(TaskMetadata::KEY_PROGRESS_MESSAGE, message);
    statusUpdate.insert(TaskMetadata::KEY_PROGRESS_VALUE, value);
    statusUpdate.insert(TaskMetadata::KEY_PROGRESS_TOTAL, total);

    setMetadata(statusUpdate);
}

void InstallAppImageInteractor::handleDownloadJobFinished() {
    downloadJob->deleteLater();
    downloadJob = nullptr;

    QFile f(installationPath);
    if (f.exists()) {
        f.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner);
    } else {
        qCritical() << "Target file not exist: " << installationPath;
    }

    setCompletedMetadata();
    emit completed();
    isRunning = false;
}

void InstallAppImageInteractor::setFailedMetadata()
{
    QVariantMap statusUpdate;

    QString d = QString("Download %1 %2 failed").arg(app.getName(), app.getVersion());
    metadata.insert(TaskMetadata::KEY_DESCRIPTION, d);

    statusUpdate.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_FAILED);
    setMetadata(statusUpdate);
}

void InstallAppImageInteractor::handleDownloadJobError(const QString &error) {
    qWarning() << "Download Error: " << app.getDownloadUrl() << " " << error;

    setFailedMetadata();

    emit completed();
    isRunning = false;
}

void InstallAppImageInteractor::handleCanceled() {
    if (downloadJob)
        downloadJob->stop();
}

void InstallAppImageInteractor::setRunningMetadata() {
    QVariantMap statusUpdate;
    statusUpdate.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_RUNNING);
    setMetadata(statusUpdate);
}

void InstallAppImageInteractor::createInstallationDirIfNotExist() {
    QDir dir = QDir::home();
    dir.mkdir("bin");
}

void InstallAppImageInteractor::createInstallationPath(QString appFileName) {
    QDir dir = QDir::home();
    dir.cd("bin");
    installationPath = dir.filePath(appFileName);
}
