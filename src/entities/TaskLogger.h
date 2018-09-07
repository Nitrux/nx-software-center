//
// Created by alexis on 26/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_REGISTRY_H
#define NOMAD_SOFTWARE_CENTER_REGISTRY_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QDateTime>
#include <QSet>

class TaskLogger : public QObject {
Q_OBJECT

    QList<QVariantMap> records;
    QDateTime expirationDate;

public:
    static constexpr const char *KEY_TIME_STAMP = "timestamp";
    static constexpr const char *KEY_IS_PERSISTENT = "persistence";

    explicit TaskLogger(QObject *parent = nullptr);

    QList<QVariantMap> getRecords() const;

    void clearRecords();

    void setExpirationDate(QDateTime date);


public slots:

    void handleTaskCompleted(const QVariantMap &task);

signals:

    void installedApplicationsChanged(const QStringList &installedApplications);

    void recordsChanged(const QList<QVariantMap> &records);

private:
    void registerDeployCompleted(QVariantMap &map);

    void registerDeployFailed(QVariantMap &map);

    void appendDeployRecord(const QVariantMap &map);

    void appendRecord(const QVariantMap &map);

    void loadRecords();

    void saveRecords();

    QByteArray serializeRecordsToJson();

    void writeJsonFile(const QByteArray &json, const QString &path);

    QString getTaskRecordsJsonPath();

    QByteArray readJsonFile(const QString &path);

    QList<QVariantMap> extractRecordsFromJson(QByteArray json);

    void registerRemoveCompleted(QVariantMap &map);

    void registerRemoveFailed(QVariantMap &map);

    void registerUpgradeCompleted(QVariantMap &map);

    void registerUpgradeFailed(QVariantMap &map);

};


#endif //NOMAD_SOFTWARE_CENTER_REGISTRY_H
