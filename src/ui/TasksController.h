//
// Created by alexis on 8/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H

#include <QObject>

#include "entities/Executor.h"
#include "TaskListModel.h"
#include <QMutex>

class TasksController : public QObject {
Q_OBJECT

    Q_PROPERTY(TaskListModel *model MEMBER model NOTIFY modelChanged);
    TaskListModel *model;
    Executor *executor;
    QMap<QString, QString> applicationsTasks;
    QMutex mutex;
public:
    TasksController(Executor *executor, QObject *parent = 0);

    Q_INVOKABLE void assignTaskToApplication(const QString &applicationId, const QString &taskId);
    Q_INVOKABLE QString getTaskOnApplication(const QString &applicationId);

signals:
    void modelChanged(TaskListModel *model);

protected slots:

    void handleTaskStarted(const QString &id);

    void handleTaskCompleted(const QString &id);

    void handleTaskDataChanged(const QString &id, const QVariantMap &data);

private:
    void removeTaskApplicationRelation(const QString &id);
};


#endif //NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H
