#include "dummyuninstallchange.h"

#include "../entities/registry.h"

DummyUninstallChange::DummyUninstallChange(QString target_release_id): DummyChange(target_release_id)
{

}


void DummyUninstallChange::finish()
{
    logs.push_front("Finished change");
    if (m_registry != nullptr)
    {
        m_registry->registerReleaseUninstall(target_release_id);

    }
    status = FINISHED;
}
