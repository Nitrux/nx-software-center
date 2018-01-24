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

class Registry : public QObject {
Q_OBJECT

    QList<QVariantMap> records;
    QMap<QString, QStringList> installedApplications;
    QDateTime expirationDate;

public:

    explicit Registry(QObject *parent = nullptr);

    QStringList getInstalledApplications() const;

    QStringList getInstalledApplicationFiles(const QString &appId);

    QList<QVariantMap> getRecords() const;

    void clearInstalledApplications();

    void clearRecords();

    void setExpirationDate(QDateTime date);


public slots:

    void handleTaskCompleted(const QString task_id, const QVariantMap resume);

signals:

    void installedApplicationsChanged(const QStringList &installedApplications);

    void recordsChanged(const QList<QVariantMap> &records);

private:
    void registerInstallCompleted(QVariantMap &map);

    void registerInstallFailed(QVariantMap &map);

    void appendInstallRecord(const QVariantMap &map);

    void removeUnneededTaskFields(QVariantMap &map) const;

    void appendRecord(const QVariantMap &map);

    void loadRecords();

    void saveRecords();

    QByteArray serializeRecordsToJson();

    void writeJsonFile(const QByteArray &json, const QString &path);

    QString getTaskRecordsJsonPath();

    QByteArray readJsonFile(const QString &path);

    QList<QVariantMap> extractRecordsFromJson(QByteArray json);

    void saveInstalledApplications();

    void loadInstalledApplications();

    QString getInstalledApplicationsPath() const;

    QByteArray serializeInstalledApplicationsToJson() const;

    void extractInstalledApplications(const QByteArray &json);

    void registerUninstallCompleted(QVariantMap &map);

    void removeInstalledApplication(QVariantMap &map);

    void registerUninstallFailed(QVariantMap &map);
};


#endif //NOMAD_SOFTWARE_CENTER_REGISTRY_H
