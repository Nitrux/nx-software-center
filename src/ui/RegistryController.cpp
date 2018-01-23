#include "RegistryController.h"

RegistryController::RegistryController(Registry *registry, QObject *parent) :
    QObject(parent), registry(registry)
{
    connect(registry, &Registry::installedApplicationsChanged,
            this, &RegistryController::handleInstalledApplicationsChanged);
}

QStringList RegistryController::getInstalledApplications()
{
    return registry->getInstalledApplications().toList();
}

void RegistryController::handleInstalledApplicationsChanged(const QSet<QString> &installedApplications)
{
    QStringList apps = installedApplications.toList();
    emit installedApplicationsChanged(apps);
}
