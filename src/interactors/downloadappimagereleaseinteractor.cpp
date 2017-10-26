#include "downloadappimagereleaseinteractor.h"

#include "downloadappimagereleaseinteractorlistener.h"

#include "../entities/app.h"
#include "../entities/release.h"
#include "../entities/registry.h"
#include "../entities/repository.h"
#include "../entities/downloadmanager.h"

DownloadAppImageReleaseInteractor::DownloadAppImageReleaseInteractor(QString appImageId, QString appImageReleaseId, QList<Repository *> repositories, DownloadManager *downloadManager, Registry *registry, DownloadAppImageReleaseInteractorListener *listener) : Interactor()
{
    m_appImageId = appImageId;
    m_appImageReleaseId = appImageReleaseId;
    m_repositories = repositories;

    Q_ASSERT(downloadManager != nullptr);
    m_downloadManager = downloadManager;

    Q_ASSERT(registry != nullptr);
    m_registry = registry;

    Q_ASSERT(listener != nullptr);
    m_listener = listener;
}

DownloadAppImageReleaseInteractor::~DownloadAppImageReleaseInteractor()
{

}


void DownloadAppImageReleaseInteractor::execute()
{
    Release *release = nullptr;
    for (Repository *repository: m_repositories)
    {
        if (repository->contains(m_appImageId, m_appImageReleaseId))
        {
            release = repository->getRelease(m_appImageId, m_appImageReleaseId);
            break;
        }
     }

    if (release == nullptr)
        m_listener->error("Release not found.");
    else if (release->download_link.isEmpty())
        m_listener->error("No download link.");
    else
    {
        QString file = m_downloadManager->download(release->download_link, m_listener);
        if (!file.isEmpty())
            m_listener->downloadComplete(file);
    }
}
