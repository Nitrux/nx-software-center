//
// Created by alexis on 8/01/18.
//

#include "TasksController.h"

#include "interactors/installappimagereleaseinteractor.h"
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

void TasksController::assignTaskToApplication(const QString &applicationId, const QString &taskId) {
    applicationsTasks.insert(applicationId, taskId);
}

QString TasksController::getTaskOnApplication(const QString &applicationId) {
    return applicationsTasks.value(applicationId, QString());
}

void TasksController::cancelTask(const QString &id)
{
    executor->cancel(id);
}

void TasksController::handleTaskStarted(const QString &id) {
    QMutexLocker locker(&mutex);
    const QVariantMap &d = executor->getTaskData(id);

    model->addTask(id, d);
}

void TasksController::handleTaskCompleted(const QString &id) {
    QMutexLocker locker(&mutex);
    removeTaskApplicationRelation(id);

    model->removeTask(id);
}

void TasksController::handleTaskDataChanged(const QString &id, const QVariantMap &data)
{
    QMutexLocker locker(&mutex);
    model->updateTask(id, data);
}

void TasksController::removeTaskApplicationRelation(const QString &id)  {
    for (const QString &key : applicationsTasks.keys()) {
        if (id == applicationsTasks.value(key)) {
            applicationsTasks.remove(key);
            break;
        }
    }
}
