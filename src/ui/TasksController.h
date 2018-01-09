//
// Created by alexis on 8/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H

#include <QObject>

#include "entities/Executor.h"

class TasksController : public QObject {
Q_OBJECT

    Executor *executor;
    QMap<QString, QString> applicationsTasks;
public:
    TasksController(Executor *executor, QObject *parent = 0) : QObject(parent), executor(executor) {}

    Q_INVOKABLE void assignTaskToApplication(const QString &applicationId, const QString &taskId);
    Q_INVOKABLE QString getTaskOnApplication(const QString &applicationId);
protected slots:

    void handleTaskStarted(const QString &id);

    void handleTaskComplete(const QString &id);

private:
    void removeTaskApplicationRelation(const QString &id);
};


#endif //NOMAD_SOFTWARE_CENTER_TASKSCONTROLLER_H
