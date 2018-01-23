//
// Created by alexis on 26/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_REGISTRY_H
#define NOMAD_SOFTWARE_CENTER_REGISTRY_H

#include <QMap>
#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QSettings>
#include <QSet>

class Registry : public QObject {
Q_OBJECT
    QList<QVariantMap> records;
    QSet<QString> installedApplications;

public:

    explicit Registry(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE QSet<QString> getInstalledApplications() const;

    Q_INVOKABLE QList<QVariantMap> getRecords() const;

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
};


#endif //NOMAD_SOFTWARE_CENTER_REGISTRY_H
