#pragma once
// system

// libraries
#include <MauiKit/FileBrowsing/downloader.h>
#include <QObject>

// local
#include <QDir>
#include <appimagetools.h>
#include <services/ApplicationData.h>
#include <services/TaskData.h>

/**
 * Perform the download and installation of AppImages one at the time
 */

class InstallWorker : public QObject
{
    Q_OBJECT

public:
    InstallWorker(const QDir &applicationsDir, const QDir &partialsDir, QObject *parent = nullptr);
    void installFromUrl(const QUrl &bundleUrl, const ApplicationData &applicationData);

    Q_SIGNAL void progressNotification(const TaskData &update);

protected:
    Q_SLOT void handleDownloadWorkerProgress(int percent);
    Q_SLOT void handleDownloadWorkerDone();
    Q_SLOT void handleDownloadWorkerError(const QString &errMsg);

private:
    bool _busy;
    QDir _applicationsDir;
    QDir _partialsDir;
    QUrl _bundleUrl;
    QString _targetFilePath;
    QString _tempFilePath;

    FMH::Downloader _downloadWorker;
    TaskData _progress;
    ApplicationData _application;

    void startFileDownload();
    void notifyDownloadStart();
    void notifyDownloadResult();
    void integrateBundle();
    void notifyInstallCompleted();
};
