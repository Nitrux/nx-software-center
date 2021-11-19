#include "TaskManager.h"
#include "UpdateTask.h"
#include "DownloadTask.h"
#include "CheckUpdateTask.h"

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

Task *TaskManager::doDownload(QUrl appDownloadUrl, QString appName)
{
    QString id = createTaskId();
    auto task = new DownloadTask(id, appName, appDownloadUrl, this);
    _tasks.push_front(task);

    emit tasksChanged(getTasks());

    task->start();
    return task;
}

Task *TaskManager::doCheckUpdate(AppsModel *appsModel)
{
    FMH::MODEL_LIST items = appsModel->getItems();

    for (int i = 0; i < items.size(); i++) {
        QString appImagePath = items[i][FMH::MODEL_KEY::PATH];
        QString appName = items[i][FMH::MODEL_KEY::NAME];

        // remove url prefix
        if (appImagePath.startsWith("file://"))
            appImagePath = appImagePath.right(appImagePath.length() - 7);

        QString id = createTaskId();
        auto task = new CheckUpdateTask(id, appImagePath, appName, appsModel, i, this);
        _tasks.push_front(task);

        emit tasksChanged(getTasks());

        task->start();
    }

    return nullptr;
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