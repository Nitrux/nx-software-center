//
// Created by alexis on 7/31/18.
//

#include <QDebug>
#include <tasks/TaskMetadata.h>

#include "UpgradeTask.h"

UpgradeTask::UpgradeTask(QObject *parent) : Task(parent), deployTask(nullptr), removeTask(nullptr) {
    type = TaskMetadata::VALUE_TYPE_UPDATE;
    status = TaskMetadata::VALUE_STATUS_RUNNING;
}

void UpgradeTask::setAppId(const QString &appId) {
    UpgradeTask::appId = appId;
}

void UpgradeTask::setDeployer(Deployer *deployer) {
    UpgradeTask::deployer = deployer;
}

void UpgradeTask::setRemover(Remover *remover) {
    UpgradeTask::remover = remover;
}

void UpgradeTask::setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry) {
    UpgradeTask::deployedApplicationsRegistry = deployedApplicationsRegistry;
}

void UpgradeTask::start() {
    if (deployTask || removeTask) {
        qCritical() << "Upgrade task already running";
        return;
    }

    appImageInfo = deployedApplicationsRegistry->getLatestDeployedVersionOf(id);
    emit Task::changed();

    deployNewAppImage();
    setStatus(TaskMetadata::VALUE_STATUS_RUNNING);
}

void UpgradeTask::stop() {
    if (deployTask) {
        deployTask->stop();
        deployTask->deleteLater();
        deployTask = nullptr;
    }

    if (removeTask) {
        removeTask->stop();
        removeTask->deleteLater();
        removeTask = nullptr;
    }
}

void UpgradeTask::deployNewAppImage() {
    deployTask = deployer->buildDeployLatestReleaseTask(appId);
    connect(deployTask, &DeployTask::completed, this, &UpgradeTask::handleDeployCompleted);
    connect(deployTask, &DeployTask::failed, this, &UpgradeTask::handleDeployFailed);
    connect(deployTask, &DeployTask::changed, this, &UpgradeTask::handleTaskChanged);

    deployTask->start();
}

void UpgradeTask::handleTaskChanged() {
    if (deployTask) {
        setProgressMessage(deployTask->getProgressMessage());
        setProgressTotal(deployTask->getProgressTotal());
        setProgressValue(deployTask->getProgressValue());
    }

    if (removeTask) {
        setProgressMessage(removeTask->getProgressMessage());
        setProgressTotal(removeTask->getProgressTotal());
        setProgressValue(removeTask->getProgressValue());
    }

    emit Task::changed();
}

void UpgradeTask::handleDeployCompleted() {
    disposeDeployTask();
    removeOldAppImage();
}

void UpgradeTask::disposeDeployTask() {
    disconnect(deployTask, nullptr, this, nullptr);
    deployTask->deleteLater();
    deployTask = nullptr;
}

void UpgradeTask::removeOldAppImage() {
    removeTask = remover->buildRemoveTaskByPath(appImageInfo.file.path);

    connect(removeTask, &RemoveTask::completed, this, &UpgradeTask::handleRemoveCompleted);
    connect(removeTask, &RemoveTask::failed, this, &UpgradeTask::handleRemoveFailed);
    connect(removeTask, &RemoveTask::changed, this, &UpgradeTask::handleTaskChanged);

    removeTask->start();
}

void UpgradeTask::handleRemoveCompleted() {
    disposeRemoveTask();
    setStatus(TaskMetadata::VALUE_STATUS_COMPLETED);
}

void UpgradeTask::disposeRemoveTask() {
    disconnect(removeTask, nullptr, this, nullptr);
    removeTask->deleteLater();
    removeTask = nullptr;
}

void UpgradeTask::handleDeployFailed() {
    disposeDeployTask();
    setStatus(TaskMetadata::VALUE_STATUS_FAILED);
}

void UpgradeTask::handleRemoveFailed() {
    disposeRemoveTask();
    setStatus(TaskMetadata::VALUE_STATUS_FAILED);
}
