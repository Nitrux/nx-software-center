//
// Created by alexis on 23/01/18.
//

#include <QDebug>

#include "Upgrader.h"
#include "Repository.h"

UpgradeList Upgrader::getUpgrades() {
    UpgradeList upgrades;

    if (repository) {
        for (const QString &appId: installedApplications) {
            try {
                const Application &current = repository->get(appId);
                const Application &latest = repository->getLatestVersion(current.getCodeName());

                if (current < latest && !installedApplications.contains(latest.getId())) {
                    QPair <QString, QString> upgrade;
                    upgrade.first = current.getId();
                    upgrade.second = latest.getId();
                    upgrades.append(upgrade);
                }
            } catch (ApplicationNotFoundException e) {
                qWarning() << e.getAppId() << " wasn't found and is reported as installed";
            }
        }
    } else
        qWarning() << "No repository set.";

    return upgrades;
}

void Upgrader::handleAvailableApplicationsChanged() {
    UpgradeList newUpgrades = getUpgrades();
    updateUpgradableApplications(newUpgrades);
}

void Upgrader::handleInstalledApplicationsChanged(const QStringList &installedApplications) {
    this->installedApplications = installedApplications;
    UpgradeList newUpgrades = getUpgrades();
    updateUpgradableApplications(newUpgrades);
}

void Upgrader::updateUpgradableApplications(const UpgradeList &newUpgrades) {
    if (newUpgrades != upgradableApplications) {
        upgradableApplications = newUpgrades;
        emit upgradableApplicationsChanged(upgradableApplications);
    }
}

Upgrader::Upgrader(QObject *parent) : QObject(parent), repository(nullptr) {

}

void Upgrader::setRepository(Repository *repository) {
    if (this->repository)
        disconnect(this->repository, &Repository::changed, this, &Upgrader::handleAvailableApplicationsChanged);

    this->repository = repository;
    connect(this->repository, &Repository::changed, this, &Upgrader::handleAvailableApplicationsChanged);
}

void Upgrader::setInstalledApplications(const QStringList &installedApplications) { this->installedApplications = installedApplications; }

UpgradeList Upgrader::getUpgradableApplications() { return upgradableApplications; }
