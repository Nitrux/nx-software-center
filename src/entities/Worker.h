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
    QQueue<Task*> queue;
public:
    void enqueueForExecution(Task* task);

signals:
    void taskStarted(const QVariantMap task);
    void taskCompleted(const QVariantMap task);
    void taskUpdated(const QVariantMap task);

protected slots:
    void startNextTask();
    void handleTaskCompleted();
    void handleTaskChanged();
};

#endif //NX_SOFTWARE_CENTER_WORKER_H
