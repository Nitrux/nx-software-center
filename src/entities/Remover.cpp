//
// Created by alexis on 7/24/18.
//

#include "Remover.h"

void Remover::setRegistry(DeployedApplicationsRegistry *registry) {
    Remover::registry = registry;
}

RemoveTask *Remover::buildRemoveTask(const QString &filePath) {
    auto application = registry->getApplicationInfo(filePath);
    auto t = new RemoveTask(application);
    return t;
}
