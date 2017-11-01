#include "registrycontroller.h"

#include "../entities/registry.h"

RegistryController::RegistryController(Registry *registry, QObject *parent) : QObject(parent)
{
    Q_ASSERT(registry != nullptr);
    m_registry = registry;
}

bool RegistryController::isReleaseDownloaded(QString appId, QString releaseId)
{
    return m_registry->isReleaseDownloaded(appId, releaseId);
}
