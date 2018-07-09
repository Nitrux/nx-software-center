#include <QUuid>
#include "Task.h"

//
// Created by alexis on 7/6/18.
//
Task::Task(QObject *parent)
        : QObject(parent), status(VALUE_STATUS_CREATED) {
    id = QUuid::createUuid().toString();
    creationTime = QDateTime::currentDateTime().currentMSecsSinceEpoch();
}

const QString &Task::getStatus() {
    QMutexLocker locker(&mutex);
    return status;
}

void Task::setStatus(const QString &status) {
    QMutexLocker locker(&mutex);
    Task::status = status;
    emit changed();
}

const QString &Task::getType() {
    QMutexLocker locker(&mutex);
    return type;
}

void Task::setType(const QString &type) {
    QMutexLocker locker(&mutex);
    Task::type = type;

    emit changed();
}

qint64 Task::getProgressValue() {
    QMutexLocker locker(&mutex);
    return progressValue;
}

void Task::setProgressValue(qint64 progressValue) {
    QMutexLocker locker(&mutex);
    Task::progressValue = progressValue;

    emit changed();
}

qint64 Task::getProgressTotal() {
    QMutexLocker locker(&mutex);
    return progressTotal;
}

void Task::setProgressTotal(qint64 progressTotal) {
    QMutexLocker locker(&mutex);
    Task::progressTotal = progressTotal;
    emit changed();
}

const QString &Task::getProgressMessage() {
    QMutexLocker locker(&mutex);
    return progressMessage;
}

void Task::setProgressMessage(const QString &progressMessage) {
    QMutexLocker locker(&mutex);
    Task::progressMessage = progressMessage;
    emit changed();
}

const ApplicationAbstract &Task::getApplicationAbstract() {
    QMutexLocker locker(&mutex);
    return applicationAbstract;
}

void Task::setApplicationAbstract(const ApplicationAbstract &applicationAbstract) {
    QMutexLocker locker(&mutex);
    Task::applicationAbstract = applicationAbstract;
    emit changed();
}

const QString &Task::getId() const {
    QMutexLocker locker(&mutex);
    return id;
}

QVariantMap Task::toVariant() const {
    QVariantMap map;
    map["id"] = id;
    map["status"] = status;
    map["type"] = type;
    map["progress_value"] = progressValue;
    map["progress_total"] = progressTotal;
    map["progress_message"] = progressMessage;
    map["application_abstract"] = applicationAbstract.toVariant();
    map["creation_time"] = creationTime;
    return map;
}

bool Task::isDeleteOnceCompletedSet() const {
    return deleteOnceCompleted;
}

void Task::setDeleteOnceCompleted(bool deleteOnceCompleted) {
    Task::deleteOnceCompleted = deleteOnceCompleted;
}

qint64 Task::getCreationTime() const {
    return creationTime;
}
