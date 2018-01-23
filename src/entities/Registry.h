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
    QSet<QString> installedApplications;
    QDateTime expirationDate;

public:

    explicit Registry(QObject *parent = nullptr);

    Q_INVOKABLE QSet<QString> getInstalledApplications() const;

    Q_INVOKABLE QList<QVariantMap> getRecords() const;

    void setExpirationDate(QDateTime date);
public slots:
    void handleTaskCompleted(const QString task_id, const QVariantMap resume);

signals:

    void installedApplicationsChanged(const QSet<QString> &installedApplications);

    void recordsChanged(const QList<QVariantMap> &records);

private:
    void registerInstallCompleted(QVariantMap map);

    void registerInstallFailed(QVariantMap map);

    void appendInstallRecord(const QVariantMap &map);

    void removeUnneededTaskFields(QVariantMap &map) const;

    void appendRecord(const QVariantMap map);

    void loadRecords();

    void saveRecords();
    QByteArray serializeRecordsToJson();
    void writeTaskRecordsJson(QByteArray json);
    QString getTaskRecordsJsonPath();
    QByteArray readTaskJsonFile();
    QList<QVariantMap> extractRecordsFromJson(QByteArray json);
    QSet<QString> extractInstalledApplications();
};


#endif //NOMAD_SOFTWARE_CENTER_REGISTRY_H
