#include "TaskManager.h"

#include <QRandomGenerator>
#include <QUuid>
#include <utility>
#include <zconf.h>

QVariant TaskManager::getTasks() const
{
    return QVariant::fromValue(_tasks);
}
TaskManager::TaskManager(QObject *parent)
    : QObject(parent)
{
}

Task *TaskManager::create(QString title, QString subtitle, QString icon, int progressTotal, int progress)
{
    QString id = QUuid::createUuid().toString();
    auto task = new Task(id, std::move(title), std::move(subtitle), std::move(icon), progressTotal, progress);

    _tasks.push_back(task);

    emit tasksChanged(getTasks());
    return task;
}
void TaskManager::destroy(Task *task)
{
    auto idx = _tasks.indexOf(task);
    if (idx >= 0) {
        _tasks.removeAt(idx);
        // schedule task for destruction
        task->deleteLater();

        emit tasksChanged(getTasks());
    }
}
