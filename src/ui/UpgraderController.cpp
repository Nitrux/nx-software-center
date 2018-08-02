//
// Created by alexis on 25/01/18.
//

#include "UpgraderController.h"
#include "entities/LocalizationUtils.h"

UpgraderController::UpgraderController(QObject *parent) : QObject(parent), upgrader(nullptr), worker(nullptr) {}

void UpgraderController::setUpgrader(Upgrader *upgrader) {
    if (upgrader)
        disconnect(upgrader, nullptr, this, nullptr);

    UpgraderController::upgrader = upgrader;
    connect(upgrader, &Upgrader::upgradesLookUpCompleted, this, &UpgraderController::handleUpgradesLookUpCompleted);
}

void UpgraderController::setWorker(Worker *worker) {
    UpgraderController::worker = worker;
}

void UpgraderController::findUpgrades() {
    upgrader->lookUpForUpgrades();
}

void UpgraderController::upgradeApplication(const QString &appId) {
    auto *task = upgrader->buildUpgradeTask(appId);
    worker->execute(task);
}

void UpgraderController::handleUpgradesLookUpCompleted() {
    upgradableApplicationIds.clear();
    const auto &upgradableApplications = upgrader->getUpgradableApplications();
    for (const auto &upgrade: upgradableApplications) {
        upgradableApplicationIds << upgrade.id;

        QVariantMap map;
        map["app_id"] = upgrade.id;
        map["app_name"] = LocalizationUtils::getLocalizedValue(upgrade.name);
        map["app_icon"] = upgrade.icon;
        if (upgrade.release.version.isEmpty())
            map["release_version"] = "Release from " + upgrade.release.date.toString(Qt::DefaultLocaleShortDate);
        else
            map["release_version"] = upgrade.release.version;

        if (upgrade.release.changelog.isEmpty())
            map["release_changelog"] = "New version available.";
        else
            map["release_changelog"] = LocalizationUtils::getLocalizedValue(upgrade.release.changelog);

        map["app_info"] = upgrade.toVariant();
        upgrades << map;
    }

    emit upgradableApplicationIdsChanged(upgradableApplicationIds);
    emit upgradesChanged(upgrades);
}
