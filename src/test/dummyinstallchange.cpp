#include "dummyinstallchange.h"

#include "../entities/registry.h"

DummyInstallChange::DummyInstallChange(QString target_release_id): DummyChange(target_release_id)
{

}


void DummyInstallChange::finish()
{
    logs.push_front("Finished change");
    if (m_registry != nullptr)
    {
        m_registry->registerReleaseDownload(target_release_id);
        m_registry->registerReleaseInstall(target_release_id);

    }
    status = FINISHED;
}
