//
// Created by alexis on 23/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_UPGRADER_H
#define NOMAD_SOFTWARE_CENTER_UPGRADER_H

#include <QObject>
#include <QPair>
#include <QList>

typedef QPair<QString, QString> Upgrade;
typedef QList<Upgrade> UpgradeList;

class Repository;

class Upgrader : public QObject {
Q_OBJECT
    Repository *repository;
    UpgradeList upgradableApplications;
    QStringList installedApplications;
    Q_PROPERTY(QList<QPair<QString, QString>> upgradableApplications
                       MEMBER upgradableApplications
                       NOTIFY upgradableApplicationsChanged)
public:
    Upgrader(QObject *parent = nullptr);

    void setRepository(Repository *repository);;

    void setInstalledApplications(const QStringList &installedApplications);

    UpgradeList getUpgradableApplications();

public slots:
    void handleInstalledApplicationsChanged(const QStringList &installedApplications);

protected slots:

    void handleAvailableApplicationsChanged();

signals:

    void upgradableApplicationsChanged(const UpgradeList &upgradableApplications);

protected:
    UpgradeList getUpgrades();
    void updateUpgradableApplications(const UpgradeList &newUpgrades);
};


#endif //NOMAD_SOFTWARE_CENTER_UPGRADER_H
