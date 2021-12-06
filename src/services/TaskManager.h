#pragma once
// system

// libraries
#include <QMap>
#include <QObject>
#include <QQmlEngine>
#include <QVariant>

// local
#include "Task.h"
#include "models/appsmodel.h"

class AppsModel;

class TaskManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant tasks READ getTasks NOTIFY tasksChanged);

public:
    explicit TaskManager(QObject *parent = nullptr);

    QVariant getTasks() const;
    Q_SIGNAL void tasksChanged(QVariant);

    Q_SCRIPTABLE Task *doUpdate(QString appImagePath, const QString& appName);
    Q_SCRIPTABLE Task *doDownload(QUrl appDownloadUrl, QString appName);
    Q_SCRIPTABLE Task *doCheckUpdate(AppsModel *appsModel);
    Q_SCRIPTABLE Task *doUpdateAll(AppsModel *appsModel);

    Q_SCRIPTABLE void destroy(Task *task);
private:
    // tasks ids used as keys
    QList<Task *> _tasks;
    QString createTaskId() const;
};
