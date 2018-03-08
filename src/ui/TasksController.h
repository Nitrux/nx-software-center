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

    Q_PROPERTY(TaskListModel *model MEMBER model NOTIFY modelChanged)
    Q_PROPERTY(QStringList affectedApplicationsIds MEMBER affectedApplicationsIds NOTIFY affectedApplicationsIdsChanged)
    TaskListModel *model;
    Executor *executor;
    QMutex mutex;
    QStringList affectedApplicationsIds;
public:
    TasksController(Executor *executor, QObject *parent = 0);

    Q_INVOKABLE void cancelTask(const QString &id);

signals:
    void modelChanged(TaskListModel *model);
    void affectedApplicationsIdsChanged(QStringList affectedApplicationsIds);

protected slots:

    void handleTaskStarted(const QString &id, const QVariantMap &data);

    void handleTaskCompleted(const QString &id, const QVariantMap &data);

    void handleTaskDataChanged(const QString &id, const QVariantMap &data);

private:
    void removeTaskApplicationRelation(const QString &id);

    bool shouldBeListed(const QVariantMap &data) const;

    bool isAnApplicationTask(const QVariantMap &data) const;

    void addAffectedApplication(const QVariantMap &data);

    void removeAffectedApplication(const QVariantMap &data);
};


#endif //NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H
