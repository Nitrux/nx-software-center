#include "UpdateTask.h"

UpdateTask::UpdateTask(const QString &id, const QString &appImagePath, const QString &appName, QObject *parent)
    : Task(id, appName, QString(), "qrc:/download.svg", -1, -1, parent)
    , _appName(appName)
    , _worker(nullptr)
{
    qDebug() << appImagePath << "  " << appName;
    _worker = new QAppImageUpdate(appImagePath, false, this);
    _worker->setAppImage(appImagePath);

    auto cancelAction = this->addCancelAction("Cancel", "dialog-cancel");
    connect(cancelAction, &TaskAction::triggered, _worker, &QAppImageUpdate::cancel);

    connect(_worker, &QAppImageUpdate::started, this, &UpdateTask::onWorkerStarted);
    connect(_worker, &QAppImageUpdate::finished, this, &UpdateTask::onWorkerFinished);
    connect(_worker, &QAppImageUpdate::progress, this, &UpdateTask::onWorkerProgress);
    connect(_worker, &QAppImageUpdate::error, this, &UpdateTask::onWorkerError);
}
void UpdateTask::start()
{
    _worker->start(QAppImageUpdate::Action::Update);
    setProgressTotal(100);

    Task::start();
}

void UpdateTask::onWorkerStarted(short)
{
    setSubtitle("Looking for new releases");
}

void UpdateTask::onWorkerFinished(QJsonObject output, short)
{
    auto newFilePath = QUrl(output.value("NewVersionPath").toString());
    auto newFileName = newFilePath.fileName();

    setStatus(Task::Status::COMPLETED);
    setTitle(newFileName);
    setSubtitle("Update completed");
    setActions({});
}
void UpdateTask::onWorkerProgress(int progressPercent, qint64, qint64, double speedValue, QString speedMeasurementUnit, short)
{
    QString progressMessage = "Downloading Update: " + QString::number(speedValue) + " " + speedMeasurementUnit;
    setSubtitle(progressMessage);
    setProgress(progressPercent);
}
void UpdateTask::onWorkerError(short, short)
{
    setStatus(Task::Status::FAILED);
    setSubtitle("Update failed");
    setActions({});
}
