//
// Created by alexis on 7/6/18.
//

#include "Task.h"
const QString& Task::getStatus()
{
    QMutexLocker locker(&mutex);
    return status;
}
void Task::setStatus(const QString& status)
{
    QMutexLocker locker(&mutex);
    Task::status = status;
}
const QString& Task::getType()
{
    QMutexLocker locker(&mutex);
    return type;
}
void Task::setType(const QString& type)
{
    QMutexLocker locker(&mutex);
    Task::type = type;
}
qint64 Task::getProgressValue()
{
    QMutexLocker locker(&mutex);
    return progressValue;
}
void Task::setProgressValue(qint64 progressValue)
{
    QMutexLocker locker(&mutex);
    Task::progressValue = progressValue;
}
qint64 Task::getProgressTotal()
{
    QMutexLocker locker(&mutex);
    return progressTotal;
}
void Task::setProgressTotal(qint64 progressTotal)
{
    QMutexLocker locker(&mutex);
    Task::progressTotal = progressTotal;
}
const QString& Task::getProgressMessage()
{
    QMutexLocker locker(&mutex);
    return progressMessage;
}
void Task::setProgressMessage(const QString& progressMessage)
{
    QMutexLocker locker(&mutex);
    Task::progressMessage = progressMessage;
}
const ApplicationAbstract& Task::getApplicationAbstract()
{
    QMutexLocker locker(&mutex);
    return applicationAbstract;
}
void Task::setApplicationAbstract(const ApplicationAbstract& applicationAbstract)
{
    QMutexLocker locker(&mutex);
    Task::applicationAbstract = applicationAbstract;
}
Task::Task(QObject* parent)
        :QObject(parent), status(VALUE_STATUS_CREATED) { }
