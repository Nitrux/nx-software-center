#include "TaskManager.h"
#include "UpdateTask.h"

#include <QAppImageUpdate>
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
    QString id = createTaskId();
    auto task = new Task(id, std::move(title), std::move(subtitle), std::move(icon), progressTotal, progress);

    _tasks.push_back(task);

    emit tasksChanged(getTasks());
    return task;
}
QString TaskManager::createTaskId() const
{
    QString id = QUuid::createUuid().toString();
    return id;
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
Task *TaskManager::doUpdate(QString appImagePath, const QString appName)
{
    QString id = createTaskId();
    if (appImagePath.startsWith("file://"))
        appImagePath = appImagePath.right(appImagePath.length() - 7);

    auto task = new UpdateTask(id, appImagePath, appName, this);
    _tasks.push_back(task);

    emit tasksChanged(getTasks());

    task->start();
    return task;
}
