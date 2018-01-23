//
// Created by alexis on 26/12/17.
//

#include "Registry.h"

#include <QDate>

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

    records.append(map);
    installedApplications.insert(app_id);

    emit installedApplicationsChanged(installedApplications);
    emit recordsChanged(records);
}

void Registry::registerInstallFailed(QVariantMap map) {
    removeUnneededTaskFields(map);

    map.insert(RecordMetadata::KEY_TIME_STAMP, QDateTime::currentDateTime());
    map.insert(RecordMetadata::KEY_IS_PERSISTENT, false);

    appendRecord(map);
}

void Registry::appendRecord(const QVariantMap map) {
    records.append(map);

    recordsChanged(records);
}

QSet<QString> Registry::getInstalledApplications() const { return  installedApplications;}

QList<QVariantMap> Registry::getRecords() const { return  records; }
