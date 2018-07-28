//
// Created by alexis on 7/24/18.
//

#include "Remover.h"

void Remover::setRegistry(DeployedApplicationsRegistry *registry) {
    Remover::registry = registry;
}

RemoveTask *Remover::buildRemoveTaskByPath(const QString &filePath) {
    auto application = registry->getApplicationInfo(filePath);
    auto t = new RemoveTask(application);
    return t;
}

RemoveTask *Remover::buildRemoveTaskById(const QString &id)
{
    auto application = registry->getLatestDeployedVersionOf(id);
    auto t = new RemoveTask(application);
    return t;
}
