//
// Created by alexis on 8/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H

#include <QObject>

#include "entities/Worker.h"
#include "TaskListModel.h"
#include <QMutex>

class TasksController : public QObject {
Q_OBJECT

    Q_PROPERTY(TaskListModel *model MEMBER model NOTIFY modelChanged)
    Q_PROPERTY(QStringList affectedApplicationsIds MEMBER affectedApplicationsIds NOTIFY affectedApplicationsIdsChanged)
    TaskListModel *model;
    Worker *worker;

    QStringList affectedApplicationsIds;
public:
    TasksController(QObject *parent = 0);

    Q_INVOKABLE void cancelTask(const QString &id);

    void setWorker(Worker *worker);

signals:

    void modelChanged(TaskListModel *model);

    void affectedApplicationsIdsChanged(QStringList affectedApplicationsIds);

protected slots:

    void handleTaskStarted(const QVariantMap &data);

    void handleTaskChanged(const QVariantMap &data);

    void handleTaskCompleted(const QVariantMap &data);

    void handleTaskFailed(const QVariantMap &data);

private:
    void removeTaskApplicationRelation(const QString &id);

    bool shouldBeListed(const QVariantMap &data) const;

    bool isAnApplicationTask(const QVariantMap &data) const;

    void addAffectedApplication(const QVariantMap &data);

    void removeAffectedApplication(const QVariantMap &data);
};


#endif //NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H
