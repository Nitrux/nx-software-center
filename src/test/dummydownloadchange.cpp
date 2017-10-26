#include "dummydownloadchange.h"

#include "../entities/registry.h"

DummyDownloadChange::DummyDownloadChange(QString appId, QString releaseId): DummyChange(appId, releaseId)
{

}


void DummyDownloadChange::finish()
{
    logs.push_front("Finished change");
    if (m_registry != nullptr)
    {
        m_registry->registerReleaseDownload(appId, releaseId, "/no_where");
    }
    status = FINISHED;
}
