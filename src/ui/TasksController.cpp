//
// Created by alexis on 8/01/18.
//

#include <QDebug>
#include <QMutexLocker>

#include <entities/Worker.h>
#include <tasks/TaskMetadata.h>
#include "TasksController.h"

TasksController::TasksController(QObject *parent)
        : QObject(parent),
          model(new TaskListModel(this)), worker(nullptr) {

}

void TasksController::cancelTask(const QString &id) {
    worker->terminate(id);
}


void TasksController::handleTaskStarted(const QVariantMap &data) {
    const QString &id = data["id"].toString();

    if (shouldBeListed(data))
        model->addTask(id, data);

    if (isAnApplicationTask(data))
        addAffectedApplication(data);
}

void TasksController::addAffectedApplication(const QVariantMap &data) {
    const auto application = data.value("application").toMap();
    const QString appId = application.value("id").toString();
    affectedApplicationsIds.append(appId);emit
    affectedApplicationsIdsChanged(affectedApplicationsIds);
}

bool TasksController::isAnApplicationTask(const QVariantMap &data) const {
    const auto application = data.value("application").toMap();
    const QString appId = application .value("id").toString();
    return !appId.isEmpty();
}

bool TasksController::shouldBeListed(const QVariantMap &data) const {
    Q_UNUSED(data)
    return true;
}

void TasksController::handleTaskCompleted(const QVariantMap &data) {
    const QString &id = data["id"].toString();
    if (shouldBeListed(data))
        model->removeTask(id);

    if (isAnApplicationTask(data))
        removeAffectedApplication(data);
}

void TasksController::removeAffectedApplication(const QVariantMap &data) {
    const auto application = data.value("application").toMap();
    const QString appId = application .value("id").toString();
    affectedApplicationsIds.removeOne(appId);emit
    affectedApplicationsIdsChanged(affectedApplicationsIds);
}

void TasksController::handleTaskChanged(const QVariantMap &data) {
    if (shouldBeListed(data)) {
        const QString &id = data["id"].toString();
        model->updateTask(id, data);
    }
}

void TasksController::setWorker(Worker *worker) {
    Q_ASSERT(worker);
    TasksController::worker = worker;

    connect(worker, &Worker::taskStarted, this, &TasksController::handleTaskStarted, Qt::QueuedConnection);
    connect(worker, &Worker::taskChanged, this, &TasksController::handleTaskChanged, Qt::QueuedConnection);
    connect(worker, &Worker::taskCompleted, this, &TasksController::handleTaskCompleted, Qt::QueuedConnection);
    connect(worker, &Worker::taskFailed, this, &TasksController::handleTaskFailed, Qt::QueuedConnection);

    auto tasks = worker->getTaskList();
    for (const auto &task : tasks) {
        const auto &map = task.toMap();
        const auto &id = map["id"].toString();
        if (shouldBeListed(map))
            model->addTask(id, map);
    }
}

void TasksController::handleTaskFailed(const QVariantMap &data) {
    const QString &id = data["id"].toString();
    if (shouldBeListed(data))
        model->removeTask(id);

    if (isAnApplicationTask(data))
        removeAffectedApplication(data);;
}
