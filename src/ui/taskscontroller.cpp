#include "taskscontroller.h"


#include <QtConcurrent>

#include "../interactors/downloadappimagereleaseinteractor.h"
#include "../interactors/removeappimagereleaseinteractor.h"

#include "taskcontroller.h"
#include "taskdownloadappimagecontroller.h"

TasksController::TasksController(QList<Repository *> repositories, Registry *registry, DownloadManager *downloadManager, QObject *parent) : QObject(parent)
{
    Q_ASSERT(registry != nullptr);
    m_registry = registry;
    Q_ASSERT(downloadManager != nullptr);
    m_downladManager = downloadManager;

    m_repositories = repositories;
}

TaskController* TasksController::getTask(QString taskId)
{
    return m_tasks.value(taskId, nullptr);
}


QString TasksController::getTaskId(QString appId, QString releaseId)
{
    QString key = appId+releaseId;
    return m_tasks.contains(key) ? key : "";
}

QString TasksController::download(QString appId, QString releaseId)
{
    QString newTaskId = appId + releaseId;

    TaskController *task = new TaskDownloadAppImageController(appId, releaseId,
                                                              m_repositories,
                                                              m_downladManager,
                                                              m_registry,
                                                              this);
    m_tasks.insert(newTaskId, task);

    task->run();

    return newTaskId;
}

QString TasksController::remove(QString appId, QString releaseId)
{
    QString newTaskId = appId + releaseId;
//    Interactor *task = new RemoveAppImageReleaseInteractor(appId, releaseId,
//                                                        m_registry, this);

//    m_tasks.insert(newTaskId, task);
//    QtConcurrent::run([=](){
//        task->execute();
//        m_tasks.remove(newTaskId);
//    });

    return newTaskId;
}
