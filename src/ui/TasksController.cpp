//
// Created by alexis on 8/01/18.
//

#include "TasksController.h"
#include <interactors/TaskMetadata.h>
#include <QMutexLocker>

TasksController::TasksController(Executor *executor, QObject *parent)
        : QObject(parent),
          model(new TaskListModel(this)),
          executor(executor) {
    connect(executor, &Executor::taskStarted, this, &TasksController::handleTaskStarted, Qt::QueuedConnection);
    connect(executor, &Executor::taskCompleted, this, &TasksController::handleTaskCompleted, Qt::QueuedConnection);
    connect(executor, &Executor::taskDataChanged, this, &TasksController::handleTaskDataChanged, Qt::QueuedConnection);

    auto tasks = executor->getRunningTasks();
    for (const QString &id : tasks) {
        const auto taskData = executor->getTaskData(id);
        if (shouldBeListed(taskData))
            model->addTask(id, taskData);
    }
}

void TasksController::cancelTask(const QString &id) {
    executor->cancel(id);
}


void TasksController::handleTaskStarted(const QString &id, const QVariantMap &data) {
    QMutexLocker locker(&mutex);

    if (shouldBeListed(data))
        model->addTask(id, data);

    if (isAnApplicationTask(data))
        addAffectedApplication(data);
}

void TasksController::addAffectedApplication(const QVariantMap &data) {
    const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();
    affectedApplicationsIds.append(appId);emit
    affectedApplicationsIdsChanged(affectedApplicationsIds);
}

bool TasksController::isAnApplicationTask(const QVariantMap &data) const {
    QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();
    return !appId.isEmpty();
}

bool TasksController::shouldBeListed(const QVariantMap &data) const {
    return data.value(TaskMetadata::KEY_TYPE) != TaskMetadata::VALUE_TYPE_UPDATE;
}

void TasksController::handleTaskCompleted(const QString &id, const QVariantMap &data) {
    QMutexLocker locker(&mutex);

    if (shouldBeListed(data))
        model->removeTask(id);

    if (isAnApplicationTask(data))
        removeAffectedApplication(data);
}

void TasksController::removeAffectedApplication(const QVariantMap &data) {
    QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();
    affectedApplicationsIds.removeOne(appId);emit
    affectedApplicationsIdsChanged(affectedApplicationsIds);
}

void TasksController::handleTaskDataChanged(const QString &id, const QVariantMap &data) {
    QMutexLocker locker(&mutex);
    if (shouldBeListed(data))
        model->updateTask(id, data);
}
