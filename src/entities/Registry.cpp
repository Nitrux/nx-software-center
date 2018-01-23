//
// Created by alexis on 26/12/17.
//

#include "Registry.h"

#include <QDate>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

#include "interactors/TaskMetadata.h"
#include "RecordMetadata.h"

void registerInstallCompleted(const QVariantMap map);

void Registry::handleTaskCompleted(const QString /*task_id*/, const QVariantMap resume) {
    const QString &taskType = resume.value(TaskMetadata::KEY_TYPE, "unknown").toString();
    const QString &taskStatus = resume.value(TaskMetadata::KEY_STATUS, "unknown").toString();
    if (taskType == TaskMetadata::VALUE_TYPE_INSTALL) {
        if (taskStatus == TaskMetadata::VALUE_STATUS_COMPLETED)
            registerInstallCompleted(resume);
        else
            registerInstallFailed(resume);
    }

    saveRecords();
}

void Registry::registerInstallCompleted(QVariantMap map) {
    removeUnneededTaskFields(map);

    map.insert(RecordMetadata::KEY_TIME_STAMP, QDateTime::currentDateTime());
    map.insert(RecordMetadata::KEY_IS_PERSISTENT, true);

    appendInstallRecord(map);
}

void Registry::removeUnneededTaskFields(QVariantMap &map) const {
    map.remove(TaskMetadata::KEY_PROGRESS_MESSAGE);
    map.remove(TaskMetadata::KEY_PROGRESS_VALUE);
    map.remove(TaskMetadata::KEY_PROGRESS_TOTAL);
    map.remove(TaskMetadata::KEY_DESCRIPTION);
}

void Registry::appendInstallRecord(const QVariantMap &map) {
    const QString app_id = map.value(TaskMetadata::KEY_APP_ID).toString();

    appendRecord(map);

    installedApplications.insert(app_id);
    emit installedApplicationsChanged(installedApplications);
}

void Registry::registerInstallFailed(QVariantMap map) {
    removeUnneededTaskFields(map);

    map.insert(RecordMetadata::KEY_TIME_STAMP, QDateTime::currentDateTime());
    map.insert(RecordMetadata::KEY_IS_PERSISTENT, false);

    appendRecord(map);
}

void Registry::appendRecord(const QVariantMap map) {
    records.push_front(map);

    recordsChanged(records);
}

QByteArray Registry::readTaskJsonFile()
{
    QByteArray json;
    QString records_path = getTaskRecordsJsonPath();

    QFile f(records_path);
    if (f.open(QIODevice::ReadOnly)) {
        json = f.readAll();
        f.close();
    }

    return json;
}

QList<QVariantMap> Registry::extractRecordsFromJson(QByteArray json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonArray recordsJsonArray = doc.array();
    QList<QVariantMap> loadedRecords;
    for (const QJsonValue &value: recordsJsonArray) {
        QJsonObject obj = value.toObject();
        QVariantMap map = obj.toVariantMap();
        loadedRecords.append(map);
    }

    return loadedRecords;
}

QSet<QString> Registry::extractInstalledApplications()
{
    QSet<QString> newInstalledApplications;
    for (const QVariantMap &map: records) {
        QString task_status = map.value(TaskMetadata::KEY_STATUS).toString();
        if (task_status.compare(TaskMetadata::VALUE_STATUS_COMPLETED) == 0) {
            // Only completed tasks are relevant
            QString task_application_id = map.value(TaskMetadata::KEY_APP_ID).toString();
            QString task_type = map.value(TaskMetadata::KEY_TYPE).toString();
            if (task_type.compare(TaskMetadata::VALUE_TYPE_INSTALL) == 0)
                newInstalledApplications.insert(task_application_id);

            if (task_type.compare(TaskMetadata::VALUE_TYPE_REMOVE) == 0)
                newInstalledApplications.remove(task_application_id);
        }
    }

    return newInstalledApplications;
}

void Registry::loadRecords()
{
    QByteArray json = readTaskJsonFile();

    records = extractRecordsFromJson(json);
    installedApplications = extractInstalledApplications();

    emit installedApplicationsChanged(installedApplications);
    emit recordsChanged(records);
}

QByteArray Registry::serializeRecordsToJson()
{
    QVariantList recordsList;
    for (const QVariantMap &map : records) {
        QString task_status = map.value(TaskMetadata::KEY_STATUS).toString();
        // Only completed tasks are relevant
        if (task_status.compare(TaskMetadata::VALUE_STATUS_COMPLETED) == 0)
            recordsList.append(map);
    }

    QJsonArray recordsJsonArray = QJsonArray::fromVariantList(recordsList);
    QJsonDocument doc(recordsJsonArray);

    QByteArray json = doc.toJson();

    return json;
}

QString Registry::getTaskRecordsJsonPath()
{
    QString records_path = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first();
    QDir::home().mkpath(records_path);
    records_path = records_path + "/tasks_records.json";

    return records_path;
}

void Registry::writeTaskRecordsJson(QByteArray json)
{
    QString records_path = getTaskRecordsJsonPath();

    QFile f(records_path);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(json);
        f.close();
    }
}

void Registry::saveRecords()
{
    QByteArray json = serializeRecordsToJson();

    writeTaskRecordsJson(json);
}

Registry::Registry(QObject *parent) : QObject(parent) {
    loadRecords();
}

QSet<QString> Registry::getInstalledApplications() const { return  installedApplications;}

QList<QVariantMap> Registry::getRecords() const { return  records; }
