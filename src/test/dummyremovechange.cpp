#include "dummyremovechange.h"

#include "../entities/registry.h"

DummyRemoveChange::DummyRemoveChange(QString target_release_id): DummyChange(target_release_id)
{

}


void DummyRemoveChange::finish()
{
    logs.push_front("Finished change");
    if (m_registry != nullptr)
    {
        m_registry->registerReleaseUninstall(target_release_id);
        m_registry->registerReleaseRemove(target_release_id);

    }
    status = FINISHED;
}
