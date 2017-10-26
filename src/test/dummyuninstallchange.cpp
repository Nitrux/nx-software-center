#include "dummyuninstallchange.h"

#include "../entities/registry.h"

DummyUninstallChange::DummyUninstallChange(QString appId, QString releaseId): DummyChange(appId, releaseId)
{

}


void DummyUninstallChange::finish()
{
    logs.push_front("Finished change");
    if (m_registry != nullptr)
    {
        m_registry->registerReleaseUninstall(appId, releaseId);

    }
    status = FINISHED;
}
