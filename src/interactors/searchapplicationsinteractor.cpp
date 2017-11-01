#include "searchapplicationsinteractor.h"

#include <QDebug>

#include <QVariant>
#include <QVariantMap>
#include <QVariantList>

#include "../entities/app.h"
#include "../entities/release.h"
#include "../entities/registry.h"
#include "../entities/repository.h"

#include "searchapplicationsinteractorlistener.h"

SearchApplicationsInteractor::SearchApplicationsInteractor(const QString &searchString, Registry *registry, QList<Repository *> repositories, SearchApplicationsInteractorListener *listener)
{
    m_searchString = searchString;

    Q_ASSERT(registry != nullptr);
    m_registry = registry;

    m_repositories = repositories;

    Q_ASSERT(listener != nullptr);
    m_listener = listener;
}

SearchApplicationsInteractor::~SearchApplicationsInteractor()
{

}

void SearchApplicationsInteractor::execute()
{
    QVariantList appsData;
    for (Repository * repository: m_repositories)
    {
        QList<App *> apps = repository->list();
        for (App * app: apps)
        {
            Release *release = app->lastRelease();
            if (m_searchString.isEmpty() ||
                    release->name.contains(m_searchString, Qt::CaseInsensitive) ||
                    release->description.contains(m_searchString, Qt::CaseInsensitive))
            {
                QVariantMap appData;
                appData["id"] = app->id;
                appData["latest_release_id"] = release->id;

                appData["name"] = release->name;
                appData["description"] = release->description;
                appData["icon_link"] = release->icon_link;
                appData["download_size"] = release->download_size;

                appData["downloaded"] = m_registry->isReleaseDownloaded(app->id, release->id);
                appData["download_link"] = release->download_link;

                appData["manual_download_link"] = app->links["Install"];

                // Dont list apps without download links
                if (release->download_link.isEmpty() &&
                        app->links["Install"].isEmpty())
                    qDebug() << app->id << " not listed.";
                else
                    appsData.append(appData);
            }
        }
    }

    if (appsData.length() > 0)
        m_listener->handleApplicationsList(appsData);
    else
        m_listener->handleNoApplicationsFound();
}
