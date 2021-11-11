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

QString TaskManager::createTaskId() const
{
    QString id = QUuid::createUuid().toString();
    return id;
}

Task *TaskManager::doUpdate(QString appImagePath, const QString &appName)
{
    // remove url prefix
    if (appImagePath.startsWith("file://"))
        appImagePath = appImagePath.right(appImagePath.length() - 7);

    QString id = createTaskId();
    auto task = new UpdateTask(id, appImagePath, appName, this);
    _tasks.push_front(task);

    emit tasksChanged(getTasks());

    task->start();
    return task;
}

Task *TaskManager::doDownload(QUrl appImageUrl)
{
    Q_UNUSED(appImageUrl);

    return nullptr;
}
