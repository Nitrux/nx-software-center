#include "taskscontroller.h"


#include <QtConcurrent>

#include "taskcontroller.h"
#include "taskdownloadappimagecontroller.h"
#include "taskremoveappimagereleasecontroller.h"
#include "taskexecuteappimagecontroller.h"

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

    TaskController *task = new TaskRemoveAppImageReleaseController(appId, releaseId,
                                                              m_registry,
                                                              this);
    m_tasks.insert(newTaskId, task);

    task->run();

    return newTaskId;
}

QString TasksController::execute(QString appId, QString releaseId)
{
    QString newTaskId = appId + releaseId;

    TaskController *task = new TaskExecuteAppImageController(appId, releaseId,
                                                              m_registry,
                                                              this);
    m_tasks.insert(newTaskId, task);

    task->run();

    return newTaskId;
}
