//
// Created by alexis on 7/4/18.
//

#include <QtCore/QDir>
#include "tasks/DeployTask.h"
#include "Deployer.h"

Deployer::Deployer()
        : repository(nullptr) {}

void Deployer::setRepository(ApplicationRepository *restClient) {
    Deployer::repository = restClient;
}

void Deployer::setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry) {
    Deployer::deployedApplicationsRegistry = deployedApplicationsRegistry;
}

DeployTask *Deployer::buildDeployLatestReleaseTask(const QString &id) {
    auto task = new DeployTask();
    task->setId(id);
    task->setRepository(repository);
    task->setApplicationsDir(QDir::homePath() + "/Applications");
    task->setDeployedApplicationsRegistry(deployedApplicationsRegistry);

    return task;
}

DeployTask *Deployer::buildDeployTask(AppImageInfo appImageInfo) {
    // TODO: Implement a direct download of the file pointed at the appImageInfo instead of downloading it again
    auto task = new DeployTask();
    task->setId(appImageInfo.id);
    task->setRepository(repository);
    task->setApplicationsDir(QDir::homePath() + "/Applications");
    task->setDeployedApplicationsRegistry(deployedApplicationsRegistry);

    return task;
}
