#include <QUuid>
#include "Task.h"
#include "AppImageInfo.h"

//
// Created by alexis on 7/6/18.
//
Task::Task(QObject *parent)
        : QObject(parent), status(VALUE_STATUS_CREATED), progressValue(-1), progressTotal(-1) {
    id = QUuid::createUuid().toString();
    creationTime = QDateTime::currentDateTime().currentMSecsSinceEpoch();
}

const QString &Task::getStatus() {
//    QMutexLocker locker(&mutex);
    return status;
}

void Task::setStatus(const QString &status) {
//    QMutexLocker locker(&mutex);
    Task::status = status;
//    emit changed();
}

const QString &Task::getType() {
//    QMutexLocker locker(&mutex);
    return type;
}

void Task::setType(const QString &type) {
//    QMutexLocker locker(&mutex);
    Task::type = type;

    emit changed();
}

qint64 Task::getProgressValue() {
//    QMutexLocker locker(&mutex);
    return progressValue;
}

void Task::setProgressValue(qint64 progressValue) {
//    QMutexLocker locker(&mutex);
    Task::progressValue = progressValue;

    emit changed();
}

qint64 Task::getProgressTotal() {
//    QMutexLocker locker(&mutex);
    return progressTotal;
}

void Task::setProgressTotal(qint64 progressTotal) {
//    QMutexLocker locker(&mutex);
    Task::progressTotal = progressTotal;
    emit changed();
}

const QString &Task::getProgressMessage() {
//    QMutexLocker locker(&mutex);
    return progressMessage;
}

void Task::setProgressMessage(const QString &progressMessage) {
//    QMutexLocker locker(&mutex);
    Task::progressMessage = progressMessage;
    emit changed();
}

const AppImageInfo & Task::getAppImageInfo() {
//    QMutexLocker locker(&mutex);
    return appImageInfo;
}

void Task::setAppImageInfo(const AppImageInfo &appImageInfo) {
//    QMutexLocker locker(&mutex);
    Task::appImageInfo = appImageInfo;
    emit changed();
}

const QString &Task::getId() {
//    QMutexLocker locker(&mutex);
    return id;
}

QVariantMap Task::toVariant(Task *task) {
    QVariantMap map;
    map["id"] = task->getId();
    map["status"] = task->getStatus();
    map["type"] = task->getType();
    map["progress_value"] = task->getProgressValue();
    map["progress_total"] = task->getProgressTotal();
    map["progress_message"] = task->getProgressMessage();
    map["application"] = task->getAppImageInfo().toVariant();
    map["creation_time"] = task->getCreationTime();
    return map;
}

bool Task::isDeleteOnceCompletedSet() {
//    QMutexLocker locker(&mutex);
    return deleteOnceCompleted;
}

void Task::setDeleteOnceCompleted(bool deleteOnceCompleted) {
//    QMutexLocker locker(&mutex);
    Task::deleteOnceCompleted = deleteOnceCompleted;
}

qint64 Task::getCreationTime() {
//    QMutexLocker locker(&mutex);
    return creationTime;
}
