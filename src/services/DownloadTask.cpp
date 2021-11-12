#include "DownloadTask.h"

DownloadTask::DownloadTask(const QString &id, const QString &appName, const QUrl &appDownloadUrl, QObject *parent)
    : Task(id, appName, QString(), "qrc:/download.svg", -1, -1, parent)
    , _appName(appName)
    , _appDownloadUrl(appDownloadUrl)
    , _worker(nullptr)
{
    qDebug() << appName << "  " << appDownloadUrl.toString();

    _worker = new FMH::Downloader(this);
    
    auto cancelAction = this->addCancelAction("Cancel", "dialog-cancel");
    connect(cancelAction, &TaskAction::triggered, _worker, [=](){
        _worker->stop();

        setStatus(Task::Status::FAILED);
        setSubtitle("Update aborted");
        setActions({});
    });

    connect(_worker, &FMH::Downloader::fileSaved, this, &DownloadTask::onWorkerFinished);
    connect(_worker, &FMH::Downloader::progress, this, &DownloadTask::onWorkerProgress);
    connect(_worker, &FMH::Downloader::warning, this, &DownloadTask::onWorkerError);
}

void DownloadTask::start()
{
    auto _appDownloadPath = NX::AppsPath.toString()+("/") + _appName;

    _worker->downloadFile(_appDownloadUrl, _appDownloadPath);
    setProgressTotal(100);

    Task::start();
}

void DownloadTask::onWorkerFinished(QString path)
{
    qDebug() << "DownloadTask::onWorkerFinished" << path;

    setStatus(Task::Status::COMPLETED);
    setSubtitle("Update completed");
    setActions({});

    // Integrate AppImage
    // if(!FMH::fileExists(path))
    //     return;

    // AppImageTools::integrate(QUrl("file://"+path).toLocalFile());
}
void DownloadTask::onWorkerProgress(const int &progress)
{
    qDebug() << "DownloadTask::onWorkerProgress" << progress;

    QString progressMessage = "Downloading Update: " + QString::number(progress) + "%";
    setSubtitle(progressMessage);
    setProgress(progress);
}
void DownloadTask::onWorkerError(QString warning)
{
    qDebug() << "DownloadTask::onWorkerError" << warning;
    setStatus(Task::Status::FAILED);
    setSubtitle("Update failed");
    setActions({});
}