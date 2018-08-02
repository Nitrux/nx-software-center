//
// Created by alexis on 23/01/18.
//

#include <QDebug>

#include "Upgrader.h"

Upgrader::Upgrader(QObject *parent) : QObject(parent), pendingRequestCount(0) {

}

void Upgrader::setApplicationRepository(ApplicationRepository *applicationRepository) {
    Upgrader::applicationRepository = applicationRepository;
}

void Upgrader::setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry) {
    Upgrader::deployedApplicationsRegistry = deployedApplicationsRegistry;
}

void Upgrader::lookUpForUpgrades() {
    Q_ASSERT(applicationRepository);
    Q_ASSERT(deployedApplicationsRegistry);

    if (pendingRequestCount > 0) {
        qWarning() << "Already looking up for upgrades.";
        return;
    }

    auto deployedApps = deployedApplicationsRegistry->listApplications();
    for (const auto &app :deployedApps) {
        auto request = applicationRepository->buildGetApplicationRequest(app.id);
        connect(request, &ApplicationRepositoryGet::completed, this,
                &Upgrader::handleApplicationRepositoryGetResult);
        connect(request, &ApplicationRepositoryGet::failed, this,
                &Upgrader::handleApplicationRepositoryGetResult);

        getApplicationsRequests << request;
        pendingRequestCount ++;
    }

    for (auto &request: getApplicationsRequests)
        request->start();
}

void Upgrader::handleApplicationRepositoryGetResult() {
    pendingRequestCount --;
    if (pendingRequestCount == 0) {
        upgradableApplications.clear();

        for (auto &request: getApplicationsRequests) {
            auto app = request->getApplication();
            auto latestReleaseInfo = app.latestCompatibleReleaseInfo();
            auto deployedReleaseInfo = deployedApplicationsRegistry->getLatestDeployedVersionOf(app.id);
            if (latestReleaseInfo.release.date > deployedReleaseInfo.release.date)
                upgradableApplications << latestReleaseInfo;
        }

        for (auto &request: getApplicationsRequests)
            request->deleteLater();

        getApplicationsRequests.clear();
        emit upgradesLookUpCompleted();
    }
}

const QList<AppImageInfo> &Upgrader::getUpgradableApplications() const {
    return upgradableApplications;
}

void Upgrader::setDeployer(Deployer *deployer) {
    Upgrader::deployer = deployer;
}

void Upgrader::setRemover(Remover *remover) {
    Upgrader::remover = remover;
}

UpgradeTask *Upgrader::buildUpgradeTask(const QString &id) {
    auto *task = new UpgradeTask();
    task->setAppId(id);
    task->setDeployer(deployer);
    task->setRemover(remover);
    task->setDeployedApplicationsRegistry(deployedApplicationsRegistry);
    task->setDeleteOnceCompleted(true);
    return  task;
}


