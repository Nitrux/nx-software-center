//
// Created by alexis on 26/12/17.
//

#include "Registry.h"

#include <QDir>
#include <QStandardPaths>
#include <QVariantList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "interactors/TaskMetadata.h"
#include "RecordMetadata.h"

static const char *const KEY_APPLICATION_ID = "appId";

static const char *const KEY_APPLICATION_INSTALLED_FILES = "files";


void Registry::handleTaskCompleted(const QString /*task_id*/, const QVariantMap resume) {
    QVariantMap map = resume;
    const QString &taskType = resume.value(TaskMetadata::KEY_TYPE, "unknown").toString();
    const QString &taskStatus = resume.value(TaskMetadata::KEY_STATUS, "unknown").toString();
    map.insert(RecordMetadata::KEY_TIME_STAMP, QDateTime::currentDateTime());
    if (taskType == TaskMetadata::VALUE_TYPE_INSTALL) {
        if (taskStatus == TaskMetadata::VALUE_STATUS_COMPLETED)
            registerInstallCompleted(map);
        else
            registerInstallFailed(map);
    }

    if (taskType == TaskMetadata::VALUE_TYPE_REMOVE) {
        if (taskStatus == TaskMetadata::VALUE_STATUS_COMPLETED)
            registerUninstallCompleted(map);
        else
            registerUninstallFailed(map);
    }

    saveRecords();
    saveInstalledApplications();
}

void Registry::registerInstallCompleted(QVariantMap &map) {
    removeUnneededTaskFields(map);

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
    const QString app_install_path = map.value(TaskMetadata::KEY_INSTALLATION_PATH).toString();

    appendRecord(map);

    QStringList files = {app_install_path};
    installedApplications.insert(app_id, files);
    emit installedApplicationsChanged(installedApplications.keys());
}

void Registry::registerInstallFailed(QVariantMap &map) {
    removeUnneededTaskFields(map);

    map.insert(RecordMetadata::KEY_IS_PERSISTENT, false);

    appendRecord(map);
}

void Registry::appendRecord(const QVariantMap &map) {
    records.push_front(map);

    emit recordsChanged(records);
}

QByteArray Registry::readJsonFile(const QString &path) {
    QByteArray json;

    QFile f(path);
    if (f.open(QIODevice::ReadOnly)) {
        json = f.readAll();
        f.close();
    }

    return json;
}

QList<QVariantMap> Registry::extractRecordsFromJson(QByteArray json) {
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

void Registry::loadRecords() {
    QString recordsPath = getTaskRecordsJsonPath();
    QByteArray json = readJsonFile(recordsPath);
    records = extractRecordsFromJson(json);
    emit recordsChanged(records);
}

QByteArray Registry::serializeRecordsToJson() {
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

QString Registry::getTaskRecordsJsonPath() {
    QString records_path = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first();
    QDir::home().mkpath(records_path);
    records_path = records_path + "/tasks_records.json";

    return records_path;
}

void Registry::writeJsonFile(const QByteArray &json, const QString &path) {
    QFile f(path);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(json);
        f.close();
    }
}

void Registry::saveRecords() {
    QByteArray json = serializeRecordsToJson();

    QString records_path = getTaskRecordsJsonPath();
    writeJsonFile(json, records_path);
}

Registry::Registry(QObject *parent) : QObject(parent) {
    expirationDate = QDateTime::currentDateTime().addMonths(-1);
    loadRecords();
    loadInstalledApplications();
}

QStringList Registry::getInstalledApplications() const { return installedApplications.keys(); }

QList<QVariantMap> Registry::getRecords() const { return records; }

void Registry::setExpirationDate(QDateTime date) {
    expirationDate = date;
}

void Registry::clearInstalledApplications() {
    installedApplications.clear();

    saveInstalledApplications();
}

void Registry::saveInstalledApplications() {
    QByteArray json = serializeInstalledApplicationsToJson();

    QString installedApplicationsPath = getInstalledApplicationsPath();
    writeJsonFile(json, installedApplicationsPath);
}

QByteArray Registry::serializeInstalledApplicationsToJson() const {
    QVariantList installedApplicationsList;
    for (const QString &appId: installedApplications.keys()) {
        QVariantMap installedApplication;
        installedApplication.insert(KEY_APPLICATION_ID, appId);
        installedApplication.insert(KEY_APPLICATION_INSTALLED_FILES, installedApplications.value(appId));

        installedApplicationsList.append(installedApplication);
    }

    QJsonArray recordsJsonArray = QJsonArray::fromVariantList(installedApplicationsList);
    QJsonDocument doc(recordsJsonArray);

    QByteArray json = doc.toJson();
    return json;
}

QString Registry::getInstalledApplicationsPath() const {
    QString installedApplicationsPath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first();
    QDir::home().mkpath(installedApplicationsPath);
    installedApplicationsPath = installedApplicationsPath + "/installedApplications.json";
    return installedApplicationsPath;
}

void Registry::loadInstalledApplications() {
    const QString installedApplicationsPath = getInstalledApplicationsPath();
    QByteArray json = readJsonFile(installedApplicationsPath);

    extractInstalledApplications(json);
}

void Registry::extractInstalledApplications(const QByteArray &json) {
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonArray array = doc.array();
    for (const QJsonValue &value: array) {
        QJsonObject object = value.toObject();
        QString appId = object.value(KEY_APPLICATION_ID).toString();
        QVariant variant = object.value(KEY_APPLICATION_INSTALLED_FILES).toVariant();
        QStringList installedFiles = variant.toStringList();

        installedApplications.insert(appId, installedFiles);
    }

    emit installedApplicationsChanged(installedApplications.keys());
}

void Registry::clearRecords() {
    records.clear();

    saveRecords();
    emit recordsChanged(records);
}

QStringList Registry::getInstalledApplicationFiles(const QString &appId) {
    return installedApplications.value(appId, QStringList());
}

void Registry::registerUninstallCompleted(QVariantMap &map) {
    removeUnneededTaskFields(map);

    map.insert(RecordMetadata::KEY_IS_PERSISTENT, true);

    appendRecord(map);

    removeInstalledApplication(map);
}

void Registry::removeInstalledApplication(QVariantMap &map) {
    const QString app_id = map.value(TaskMetadata::KEY_APP_ID).toString();
    installedApplications.remove(app_id);
    emit installedApplicationsChanged(installedApplications.keys());

}

void Registry::registerUninstallFailed(QVariantMap &map) {
    removeUnneededTaskFields(map);

    map.insert(RecordMetadata::KEY_IS_PERSISTENT, false);

    appendRecord(map);
}
