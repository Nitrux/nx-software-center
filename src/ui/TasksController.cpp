//
// Created by alexis on 8/01/18.
//

#include "TasksController.h"

#include "interactors/installappimagereleaseinteractor.h"

TasksController::TasksController(Executor *executor, QObject *parent)
    : QObject(parent),
      executor(executor),
      model(new TaskListModel(this))
{
    connect(executor, &Executor::taskStarted, this, &TasksController::handleTaskStarted);
    connect(executor, &Executor::taskCompleted, this, &TasksController::handleTaskCompleted);
    connect(executor, &Executor::taskDataChanged, this, &TasksController::handleTaskDataChanged);
}

void TasksController::assignTaskToApplication(const QString &applicationId, const QString &taskId) {
    applicationsTasks.insert(applicationId, taskId);
}

QString TasksController::getTaskOnApplication(const QString &applicationId) {
    return applicationsTasks.value(applicationId, QString());
}

void TasksController::handleTaskStarted(const QString &id) {
    const QVariantMap &d = executor->getTaskData(id);

    model->addTask(id, d);
}

void TasksController::handleTaskCompleted(const QString &id) {
    removeTaskApplicationRelation(id);

    model->removeTask(id);
}

void TasksController::handleTaskDataChanged(const QString &id, const QVariantMap &data)
{
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
