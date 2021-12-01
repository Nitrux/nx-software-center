#include "DownloadTask.h"

DownloadTask::DownloadTask(const QString &id, const QString &appName, const QUrl &appDownloadUrl, TaskManager *taskManager)
    : Task(id, appName, QString(), "qrc:/download.svg", -1, -1, taskManager)
    , _appName(appName)
    , _appDownloadUrl(appDownloadUrl)
    , _worker(nullptr)
    , _taskManager(taskManager)
{
    qDebug() << appName << "  " << appDownloadUrl.toString();

    _worker = new FMH::Downloader(this);
    
    auto cancelAction = this->addCancelAction("Cancel", "dialog-cancel");
    connect(cancelAction, &TaskAction::triggered, _worker, [=](){
        _worker->stop();

        setStatus(Task::Status::FAILED);
        setSubtitle("Download aborted");
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
    setSubtitle("Download completed");
    setActions({});

    // Integrate AppImage
    if(!FMH::fileExists(QUrl::fromLocalFile(path)))
        return;

    AppImageTools::integrate(QUrl::fromLocalFile(path));

    auto action = addAction("dismiss", "Dismiss", "user-trash");
    action->setIsActive(true);
    connect(action, &TaskAction::triggered, [=]() {
        _taskManager->destroy(this);
    });
}
void DownloadTask::onWorkerProgress(const int &progress)
{
    qDebug() << "DownloadTask::onWorkerProgress" << progress;

    QString progressMessage = "Downloading: " + QString::number(progress) + "%";
    setSubtitle(progressMessage);
    setProgress(progress);
}
void DownloadTask::onWorkerError(QString warning)
{
    qDebug() << "DownloadTask::onWorkerError" << warning;
    setStatus(Task::Status::FAILED);
    setSubtitle("Download failed");
    setActions({});
}