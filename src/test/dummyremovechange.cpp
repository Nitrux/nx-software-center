#include "dummyremovechange.h"

#include "../entities/registry.h"

DummyRemoveChange::DummyRemoveChange(QString appId, QString releaseId): DummyChange(appId, releaseId)
{

}


void DummyRemoveChange::finish()
{
    logs.push_front("Finished change");
    if (m_registry != nullptr)
    {
        m_registry->registerReleaseUninstall(appId, releaseId);
        m_registry->registerReleaseRemove(appId, releaseId);

    }
    status = FINISHED;
}
