#include "InstallWorker.h"

#include <QDebug>

InstallWorker::InstallWorker(const QDir &applicationsDir, const QDir &partialsDir, QObject *parent)
    : QObject(parent)
    , _busy(false)
    , _applicationsDir(applicationsDir)
    , _partialsDir(partialsDir)
    , _downloadWorker(this)
    , _application()
{
    connect(&_downloadWorker, &FMH::Downloader::progress, this, &InstallWorker::handleDownloadWorkerProgress);
    connect(&_downloadWorker, &FMH::Downloader::done, this, &InstallWorker::handleDownloadWorkerDone);
    connect(&_downloadWorker, &FMH::Downloader::warning, this, &InstallWorker::handleDownloadWorkerError);
}

void InstallWorker::installFromUrl(const QUrl &bundleUrl, const Application &app)
{
    if (_busy) {
        qWarning() << "InstallWorker::install unable to start another task while busy";
        return;
    }
    _busy = true;

    _bundleUrl = bundleUrl;
    _targetFilePath = _applicationsDir.absoluteFilePath(_bundleUrl.fileName());
    _tempFilePath = _partialsDir.path() + "/" + _bundleUrl.fileName();
    _application = app;

    _progress = TaskData();

    const auto &appData = app.getData();
    _progress.iconPath = appData.getIcon();
    _progress.title = "Installing " + appData.getName();

    startFileDownload();
}

void InstallWorker::startFileDownload()
{
    _downloadWorker.downloadFile(_bundleUrl, QUrl::fromLocalFile(_tempFilePath));
    notifyDownloadStart();
}

void InstallWorker::handleDownloadWorkerProgress(int percent)
{
    _progress.current_progress = percent;
    emit(progressNotification(_progress));
}

void InstallWorker::notifyDownloadStart()
{
    _progress.current_progress = 0;
    _progress.total_progress = 100;
    _progress.status = TaskData::RUNNING;
    _progress.subTitle = "Downloading bundle";

    _progress.related_app_id = _application.getId();
    emit(progressNotification(_progress));
}

void InstallWorker::handleDownloadWorkerDone()
{
    notifyDownloadResult();
    verifyBundle();
}

void InstallWorker::notifyDownloadResult()
{
    _progress.current_progress = -1;
    _progress.total_progress = -1;

    if (QFile::exists(_tempFilePath)) {
        _progress.subTitle = "Download completed";
    } else {
        _progress.subTitle = "Download failed";
    }
}

void InstallWorker::verifyBundle()
{
    QFile tempFile(_tempFilePath);
    if (tempFile.exists())
        tempFile.rename(_targetFilePath);

    notifyInstallCompleted();
}

void InstallWorker::notifyInstallCompleted()
{
    _progress.current_progress = 0;
    _progress.total_progress = 0;
    _progress.subTitle = "Installation completed";
    _progress.status = TaskData::SUCCEED;
    emit(progressNotification(_progress));
}

void InstallWorker::handleDownloadWorkerError(const QString &errMsg)
{
    _progress.current_progress = 0;
    _progress.total_progress = 0;

    _progress.subTitle = errMsg;
    _progress.status = TaskData::FAILED;
    emit(progressNotification(_progress));
}
