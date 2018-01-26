//
// Created by alexis on 8/01/18.
//

#include "TasksController.h"
#include <interactors/TaskMetadata.h>
#include <QMutexLocker>

TasksController::TasksController(Executor *executor, QObject *parent)
    : QObject(parent),
      model(new TaskListModel(this)),
      executor(executor)
{
    connect(executor, &Executor::taskStarted, this, &TasksController::handleTaskStarted, Qt::QueuedConnection);
    connect(executor, &Executor::taskCompleted, this, &TasksController::handleTaskCompleted, Qt::QueuedConnection);
    connect(executor, &Executor::taskDataChanged, this, &TasksController::handleTaskDataChanged, Qt::QueuedConnection);

    auto tasks = executor->getRunningTasks();
    for (const QString &id : tasks)
        model->addTask(id, executor->getTaskData(id));
}

void TasksController::cancelTask(const QString &id)
{
    executor->cancel(id);
}


void TasksController::handleTaskStarted(const QString &id) {
    QMutexLocker locker(&mutex);
    const QVariantMap &d = executor->getTaskData(id);

    model->addTask(id, d);

    QString appId = d.value(TaskMetadata::KEY_APP_ID).toString();
    if (!appId.isEmpty()) {
        affectedApplicationsIds.append(appId);
        emit affectedApplicationsIdsChanged(affectedApplicationsIds);
    }
}

void TasksController::handleTaskCompleted(const QString &id) {
    QMutexLocker locker(&mutex);

    const QVariantMap d = model->getTask(id);

    model->removeTask(id);

    QString appId = d.value(TaskMetadata::KEY_APP_ID).toString();
    if (!appId.isEmpty()) {
        affectedApplicationsIds.removeOne(appId);
        emit affectedApplicationsIdsChanged(affectedApplicationsIds);
    }
}

void TasksController::handleTaskDataChanged(const QString &id, const QVariantMap &data)
{
    QMutexLocker locker(&mutex);
    model->updateTask(id, data);
}
