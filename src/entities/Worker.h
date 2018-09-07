//
// Created by alexis on 7/6/18.
//

#ifndef NX_SOFTWARE_CENTER_WORKER_H
#define NX_SOFTWARE_CENTER_WORKER_H

#include <QObject>
#include <QQueue>
#include "Task.h"

class Worker : public QObject {
Q_OBJECT
    QMap<QString, Task *> tasks;
    QMutex mutex;
public:
    void execute(Task *task);
    void terminate(const QString taskId);

    QVariantList getTaskList();
signals:

    void taskStarted(const QVariantMap task);

    void taskCompleted(const QVariantMap task);

    void taskFailed(const QVariantMap task);

    void taskChanged(const QVariantMap task);

protected slots:

    void handleTaskChanged();

    void handleTaskCompleted();

    void handleTaskFailed();

    void disposeTask(Task *task);
};

#endif //NX_SOFTWARE_CENTER_WORKER_H
