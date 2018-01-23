#include "RegistryController.h"

RegistryController::RegistryController(Registry *registry, QObject *parent) :
    QObject(parent), registry(registry), model(new RegistryListModel(this))
{
    connect(registry, &Registry::installedApplicationsChanged,
            this, &RegistryController::handleInstalledApplicationsChanged);

    connect(registry, &Registry::recordsChanged,
            this, &RegistryController::handleRecordsChanged);
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

void RegistryController::handleRecordsChanged(const QList<QVariantMap> &records)
{
    model->setRecords(records);
}
