#include "RegistryController.h"

RegistryController::RegistryController(Registry *registry, QObject *parent) :
    QObject(parent), registry(registry), model(new RegistryListModel(this))
{
    model->setRecords(registry->getRecords());

    connect(registry, &Registry::installedApplicationsChanged,
            this, &RegistryController::installedApplicationsChanged);

    connect(registry, &Registry::recordsChanged,
            this, &RegistryController::handleRecordsChanged);
}

QStringList RegistryController::getInstalledApplications()
{
    return registry->getInstalledApplications();
}

void RegistryController::handleRecordsChanged(const QList<QVariantMap> &records)
{
    model->setRecords(records);
}

void RegistryController::clearRecords() {
    registry->clearRecords();
}
