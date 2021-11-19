#include "CheckUpdateTask.h"

CheckUpdateTask::CheckUpdateTask(const QString &id, const QString &appImagePath, const QString &appName, AppsModel *appsModel, int index, QObject *parent)
    : Task(id, appName, QString(), "qrc:/download.svg", -1, -1, parent)
    , _appName(appName)
    , _worker(nullptr)
{
    qDebug() << appImagePath << "  " << appName;
    _worker = new QAppImageUpdate(appImagePath, false, this);
    _worker->setAppImage(appImagePath);

    auto cancelAction = this->addCancelAction("Cancel", "dialog-cancel");
    connect(cancelAction, &TaskAction::triggered, _worker, &QAppImageUpdate::cancel);

    connect(_worker, &QAppImageUpdate::started, this, &CheckUpdateTask::onWorkerStarted);
    connect(_worker, &QAppImageUpdate::finished, this, [=](QJsonObject output, short action){
        onWorkerFinished(output, action, appsModel, index);
    });
    connect(_worker, &QAppImageUpdate::progress, this, &CheckUpdateTask::onWorkerProgress);
    connect(_worker, &QAppImageUpdate::error, this, &CheckUpdateTask::onWorkerError);
}
void CheckUpdateTask::start()
{
    _worker->start(QAppImageUpdate::Action::CheckForUpdate);
    setProgressTotal(100);

    Task::start();
}

void CheckUpdateTask::onWorkerStarted(short)
{
    setSubtitle("Checking for update");
}

void CheckUpdateTask::onWorkerFinished(QJsonObject output, short, AppsModel *appsModel, int index)
{
    auto isUpdateAvailable = output.value("UpdateAvailable");

    setStatus(Task::Status::COMPLETED);
    if ( isUpdateAvailable == true ) {
        setSubtitle("New update available");
        appsModel->setAppUpdatable(index);
    } else {
        setSubtitle("App is already updated and latest");
    }
    setActions({});
}
void CheckUpdateTask::onWorkerProgress(int progressPercent, qint64, qint64, double, QString, short)
{
    setProgress(progressPercent);
}
void CheckUpdateTask::onWorkerError(short errorCode, short)
{
    setStatus(Task::Status::FAILED);
    setSubtitle(QAppImageUpdate::errorCodeToString(errorCode));
    setActions({});
}