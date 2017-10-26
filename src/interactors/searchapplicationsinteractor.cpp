#include "searchapplicationsinteractor.h"

#include <QVariant>
#include <QVariantMap>
#include <QVariantList>

#include "../entities/app.h"
#include "../entities/release.h"
#include "../entities/repository.h"

#include "searchapplicationsinteractorlistener.h"

SearchApplicationsInteractor::SearchApplicationsInteractor(const QString &searchString, QList<Repository *> repositories, SearchApplicationsInteractorListener *listener)
{
    m_searchString = searchString;
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
                    release->name.contains(m_searchString) ||
                    release->description.contains(m_searchString))
            {
                QVariantMap appData;
                appData["id"] = app->id;
                appData["latest_release_id"] = release->id;

                appData["name"] = release->name;
                appData["description"] = release->description;
                appData["icon_link"] = release->icon_link;
                appData["download_size"] = release->download_size;

                appsData.append(appData);
            }
        }
    }

    if (appsData.length() > 0)
        m_listener->handleApplicationsList(appsData);
    else
        m_listener->handleNoApplicationsFound();
}
