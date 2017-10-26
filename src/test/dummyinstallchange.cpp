#include "dummyinstallchange.h"

#include "../entities/registry.h"

DummyInstallChange::DummyInstallChange(QString appId, QString releaseId): DummyChange(appId, releaseId)
{

}


void DummyInstallChange::finish()
{
    logs.push_front("Finished change");
    if (m_registry != nullptr)
    {
        m_registry->registerReleaseDownload(appId, releaseId, "/no_where");
        m_registry->registerReleaseInstall(appId, releaseId, QStringList({"/tmp/one", "/tmp/two"}));

    }
    status = FINISHED;
}
