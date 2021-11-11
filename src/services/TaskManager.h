#pragma once
// system

// libraries
#include <QMap>
#include <QObject>
#include <QQmlEngine>
#include <QVariant>

// local
#include "Task.h"

class TaskManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant tasks READ getTasks NOTIFY tasksChanged);

public:
    explicit TaskManager(QObject *parent = nullptr);

    QVariant getTasks() const;
    Q_SIGNAL void tasksChanged(QVariant);

    Q_SCRIPTABLE Task *doUpdate(QString appImagePath, const QString& appName);

private:
    // tasks ids used as keys
    QList<Task *> _tasks;
    QString createTaskId() const;
};
