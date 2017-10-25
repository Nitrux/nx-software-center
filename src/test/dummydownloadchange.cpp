#include "dummydownloadchange.h"

#include "../entities/registry.h"

DummyDownloadChange::DummyDownloadChange(QString target_release_id): DummyChange(target_release_id)
{

}


void DummyDownloadChange::finish()
{
    logs.push_front("Finished change");
    if (m_registry != nullptr)
    {
        m_registry->registerReleaseDownload(target_release_id);
    }
    status = FINISHED;
}
