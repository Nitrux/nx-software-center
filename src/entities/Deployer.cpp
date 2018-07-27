//
// Created by alexis on 7/4/18.
//

#include <QtCore/QDir>
#include "DeployTask.h"
#include "Deployer.h"

Deployer::Deployer()
        : repository(nullptr) {}

void Deployer::setRepository(ApplicationRepository *restClient) {
    Deployer::repository = restClient;
}

DeployTask *Deployer::buildInstallLatestReleaseTask(const QString &id) {
    auto task = new DeployTask();
    task->setId(id);
    task->setRepository(repository);
    task->setApplicationsDir(QDir::homePath() + "/Applications");

    return task;
}
