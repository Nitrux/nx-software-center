//
// Created by alexis on 26/12/17.
//

#include "TaskLogger.h"

#include <QDir>
#include <QStandardPaths>
#include <QVariantList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "entities/LocalizationUtils.h"
#include "tasks/TaskMetadata.h"

static const char *const KEY_APPLICATION_ID = "appId";

static const char *const KEY_APPLICATION_INSTALLED_FILES = "files";


void TaskLogger::handleTaskCompleted(const QVariantMap &task) {
    QVariantMap record;
    const auto &app = task["application"].toMap();
    const QString &taskType = task[TaskMetadata::KEY_TYPE].toString();
    record[TaskMetadata::KEY_TYPE] = task[TaskMetadata::KEY_TYPE];
    const QString &taskStatus = task[TaskMetadata::KEY_STATUS].toString();
    record[TaskMetadata::KEY_STATUS] = task[TaskMetadata::KEY_STATUS];
    record[KEY_TIME_STAMP] = QDateTime::currentDateTime();

    record[TaskMetadata::KEY_APP_ID] = app["id"].toString();
    record[TaskMetadata::KEY_APP_NAME] = LocalizationUtils::getLocalizedValue(app["name"].toMap());


    const auto &release = app["release"].toMap();
    record[TaskMetadata::KEY_APP_VERSION] = release["version"];

    const auto &developer = app["developer"].toMap();
    record[TaskMetadata::KEY_APP_AUTHOR] = developer["name"];

    if (taskType == TaskMetadata::VALUE_TYPE_DELPOY) {
        if (taskStatus == TaskMetadata::VALUE_STATUS_COMPLETED)
            registerDeployCompleted(record);
        else
            registerDeployFailed(record);
    }

    if (taskType == TaskMetadata::VALUE_TYPE_REMOVE) {
        if (taskStatus == TaskMetadata::VALUE_STATUS_COMPLETED)
            registerRemoveCompleted(record);
        else
            registerRemoveFailed(record);
    }

    if (taskType == TaskMetadata::VALUE_TYPE_UPDATE) {
        if (taskStatus == TaskMetadata::VALUE_STATUS_COMPLETED)
            registerUpgradeCompleted(record);
        else
            registerUpgradeFailed(record);
    }

    saveRecords();
}

void TaskLogger::registerDeployCompleted(QVariantMap &map) {

    map[TaskMetadata::KEY_DESCRIPTION] = "Application installed successfully";
    map[KEY_IS_PERSISTENT] = true;

    appendDeployRecord(map);
}

void TaskLogger::appendDeployRecord(const QVariantMap &map) {
    const QString app_id = map[TaskMetadata::KEY_APP_ID].toString();
    const QString app_install_path = map.value(TaskMetadata::KEY_INSTALLATION_PATH).toString();

    appendRecord(map);
}

void TaskLogger::registerDeployFailed(QVariantMap &map) {
    map.insert(KEY_IS_PERSISTENT, false);
    map[TaskMetadata::KEY_DESCRIPTION] = "Application install failed";

    appendRecord(map);
}

void TaskLogger::appendRecord(const QVariantMap &map) {
    records.push_front(map);

    emit recordsChanged(records);
}

QByteArray TaskLogger::readJsonFile(const QString &path) {
    QByteArray json;

    QFile f(path);
    if (f.open(QIODevice::ReadOnly)) {
        json = f.readAll();
        f.close();
    }

    return json;
}

QList<QVariantMap> TaskLogger::extractRecordsFromJson(QByteArray json) {
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

void TaskLogger::loadRecords() {
    QString recordsPath = getTaskRecordsJsonPath();
    QByteArray json = readJsonFile(recordsPath);
    records = extractRecordsFromJson(json);
    emit recordsChanged(records);
}

QByteArray TaskLogger::serializeRecordsToJson() {
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

QString TaskLogger::getTaskRecordsJsonPath() {
    QString records_path = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first();
    QDir::home().mkpath(records_path);
    records_path = records_path + "/tasks_records.json";

    return records_path;
}

void TaskLogger::writeJsonFile(const QByteArray &json, const QString &path) {
    QFile f(path);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(json);
        f.close();
    }
}

void TaskLogger::saveRecords() {
    QByteArray json = serializeRecordsToJson();

    QString records_path = getTaskRecordsJsonPath();
    writeJsonFile(json, records_path);
}

TaskLogger::TaskLogger(QObject *parent) : QObject(parent) {
    expirationDate = QDateTime::currentDateTime().addMonths(-1);
    loadRecords();
}

QList<QVariantMap> TaskLogger::getRecords() const { return records; }

void TaskLogger::setExpirationDate(QDateTime date) {
    expirationDate = date;
}

void TaskLogger::clearRecords() {
    records.clear();

    saveRecords();
    emit recordsChanged(records);
}

void TaskLogger::registerRemoveCompleted(QVariantMap &map) {
    map[TaskMetadata::KEY_DESCRIPTION] = "Application removed successfully";
    map.insert(KEY_IS_PERSISTENT, true);

    appendRecord(map);
}

void TaskLogger::registerRemoveFailed(QVariantMap &map) {
    map[TaskMetadata::KEY_DESCRIPTION] = "Application remove failed";
    map.insert(KEY_IS_PERSISTENT, false);

    appendRecord(map);
}

void TaskLogger::registerUpgradeCompleted(QVariantMap &map)
{
    map[TaskMetadata::KEY_DESCRIPTION] = "Application upgraded successfully";
    map.insert(KEY_IS_PERSISTENT, true);

    appendRecord(map);
}

void TaskLogger::registerUpgradeFailed(QVariantMap &map)
{
    map[TaskMetadata::KEY_DESCRIPTION] = "Application upgraded failed";
    map.insert(KEY_IS_PERSISTENT, false);

    appendRecord(map);
}
