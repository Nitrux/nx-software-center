#include "CheckUpdateTask.h"

CheckUpdateTask::CheckUpdateTask(const QString &id, const QString &appImagePath, const QString &appName, AppsModel *appsModel, int index, QObject *parent)
    : Task(id, appName, QString(), "qrc:/download.svg", -1, -1, parent)
    , _appName(appName)
    , _worker(nullptr)
    , _taskManager(dynamic_cast<TaskManager*>(parent))
    , _appsModel(appsModel)
    , _index(index)
{
    qDebug() << appImagePath << "  " << appName;
    _worker = new Updater(appImagePath.toStdString());
    
    // auto cancelAction = this->addCancelAction("Cancel", "dialog-cancel");
    // connect(cancelAction, &TaskAction::triggered, _worker, [=](){
    //     setStatus(Task::Status::FAILED);
    //     setSubtitle("Download aborted");
    //     setActions({});
    // });
}
void CheckUpdateTask::start()
{
    setProgressTotal(100);
    Task::start();

    std::thread processCheckUpdateThread(&CheckUpdateTask::processUpdate, this, _appsModel, _index);
    processCheckUpdateThread.detach();
}

void CheckUpdateTask::processUpdate(AppsModel *appsModel, int index) {
    bool updateAvailable;
    if (!_worker->checkForChanges(updateAvailable)) {
        setStatus(Task::Status::FAILED);
        setSubtitle("Update not available");
        setActions({});

        _taskManager->destroy(this);

        return ;
    }

    if (updateAvailable) {
        setStatus(Task::Status::COMPLETED);
        setSubtitle("New update available");

        appsModel->setAppUpdatable(index);
    } else {
        qDebug() << "Update not available for app";

        setStatus(Task::Status::FAILED);
        setSubtitle("Update not available");
        setActions({});

        _taskManager->destroy(this);

        return ;
    }
}