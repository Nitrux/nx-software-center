//
// Created by alexis on 7/6/18.
//
#include <tasks/TaskMetadata.h>
#include <entities/Task.h>
#include "Worker.h"


void Worker::handleTaskCompleted() {
    Task *task;
    task = dynamic_cast<Task *>(sender());
    if (task) {
        QMutexLocker locker(&mutex);

        emit taskCompleted(Task::toVariant(task));
        disposeTask(task);
    }
}

void Worker::handleTaskChanged() {
    Task *task;
    task = dynamic_cast<Task *>(sender());
    if (task)
            emit taskChanged(Task::toVariant(task));
}

QVariantList Worker::getTaskList() {
    QVariantList tasksList;
    for (const auto &task: tasks.values())
        tasksList << Task::toVariant(task);

    return tasksList;
}

void Worker::execute(Task *task) {
    Q_ASSERT(task);
    QMutexLocker locker(&mutex);

    const auto &taskId = task->getId();
    tasks.insert(taskId, task);
    connect(task, &Task::changed, this, &Worker::handleTaskChanged);
    connect(task, &Task::completed, this, &Worker::handleTaskCompleted);
    connect(task, &Task::failed, this, &Worker::handleTaskFailed);

    task->start();
    emit taskStarted(Task::toVariant(task));
}

void Worker::terminate(const QString taskId) {
    if (tasks.contains(taskId)) {
        QMutexLocker locker(&mutex);

        auto task = tasks.value(taskId);
        task->stop();
    }

}

void Worker::handleTaskFailed() {
    Task *task;
    task = dynamic_cast<Task *>(sender());
    if (task) {
        QMutexLocker locker(&mutex);

        emit taskFailed(Task::toVariant(task));

        disposeTask(task);
    }
}

void Worker::disposeTask(Task *task) {
    if (task->isDeleteOnceCompletedSet()) {
        const QString &id = task->getId();
        tasks.remove(id);

        task->deleteLater();
    }
}
