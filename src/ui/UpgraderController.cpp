//
// Created by alexis on 25/01/18.
//

#include <interactors/RemoveAppImageInteractor.h>
#include <interactors/InstallAppImageInteractor.h>
#include "UpgraderController.h"
#include "UpgradesListModel.h"

UpgraderController::UpgraderController(Upgrader *upgrader, Repository *repository, Registry *registry,
                                       Executor *executor, DownloadManager *downloadManager, QObject *parent)
        : QObject(parent), upgrader(upgrader), repository(repository),
          registry(registry), executor(executor),
          downloadManager(downloadManager), model(new UpgradesListModel(this)) {

    updateApplicationsModel(upgrader->getUpgradableApplications());
    connect(upgrader, &Upgrader::upgradableApplicationsChanged, this,
            &UpgraderController::handleUpgradableApplicationsChanged);

}

void UpgraderController::upgrade(const QString &appCodeName) {
    const UpgradeList &upgrades = upgrader->getUpgradableApplications();
    QString oldAppId, newAppId;
    for (const Upgrade &upgrade: upgrades) {
        if (upgrade.first.startsWith(appCodeName) &&
            upgrade.second.startsWith(appCodeName)) {
            oldAppId = upgrade.first;
            newAppId = upgrade.second;
        }
    }

    if (!oldAppId.isEmpty() && !newAppId.isEmpty()) {
        upgrade(oldAppId, newAppId);
    }
}

void UpgraderController::upgrade(const QString &oldAppId, const QString &newAppId) {
    InstallAppImageInteractor *i = executeInstall(newAppId);

    if (i) {
        connect(i, &Interactor::completed, [=]() {
            const QVariantMap metadata = i->getMetadata();
            if (isTaskCompletedSuccesfully(metadata))
                executeRemove(oldAppId);
        });
    }
}

bool UpgraderController::isTaskCompletedSuccesfully(const QVariantMap &metadata) const {
    const QString &result = metadata.value(TaskMetadata::KEY_STATUS).toString();
    return result.compare(TaskMetadata::VALUE_STATUS_COMPLETED) == 0;
}

void UpgraderController::executeRemove(const QString &oldAppId) const {
    try {
        const Application &a = repository->get(oldAppId);
        const QStringList &files = registry->getInstalledApplicationFiles(oldAppId);
        auto *i = new RemoveAppImageInteractor(a, files);
        executor->execute(i);
    } catch (ApplicationNotFoundException e) {
        qCritical() << "Application " << e.getAppId() << " not found in repository.";
    }
}

InstallAppImageInteractor *UpgraderController::executeInstall(const QString &newAppId) const {
    InstallAppImageInteractor *i = nullptr;
    try {
        const Application &a = repository->get(newAppId);
        i = new InstallAppImageInteractor(a, downloadManager);
        executor->execute(i);
    } catch (ApplicationNotFoundException e) {
        qCritical() << "Application " << e.getAppId() << " not found in repository.";
    }
    return i;
}

void UpgraderController::updateApplicationsModel(const UpgradeList &upgradableApplications) {
    this->upgradableApplications.clear();

    QList<QMap<int, QVariant>> newUpgrades;
    for (const Upgrade &upgrade: upgradableApplications) {
        try {
            const Application &oldApp = repository->get(upgrade.first);
            const Application &newApp = repository->get(upgrade.second);

            QMap<int, QVariant> upgradeData;
            upgradeData[UpgradesListModel::CodeName] = oldApp.getCodeName();
            upgradeData[UpgradesListModel::OldId] = oldApp.getId();
            upgradeData[UpgradesListModel::NewId] = newApp.getId();
            upgradeData[UpgradesListModel::NewName] = newApp.getName();
            upgradeData[UpgradesListModel::NewDescription] = newApp.getDescription();
            upgradeData[UpgradesListModel::NewVersion] = newApp.getVersion();
            // TODO: add changelog

            newUpgrades.append(upgradeData);
            this->upgradableApplications.append(oldApp.getCodeName());
        } catch (ApplicationNotFoundException e) {
            qCritical() << "Application not found: " << e.getAppId();
        }
    }

    model->setUpgrades(newUpgrades);
    emit upgradableApplicationsChanged(this->upgradableApplications);
}

void UpgraderController::handleUpgradableApplicationsChanged(const UpgradeList &upgradableApplications) {
    updateApplicationsModel(upgradableApplications);

    emit upgradableApplicationsChanged(this->upgradableApplications);
}
