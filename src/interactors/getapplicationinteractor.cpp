#include "getapplicationinteractor.h"

#include "../entities/app.h"
#include "../entities/release.h"
#include "../entities/repository.h"

GetApplicationInteractor::GetApplicationInteractor(QString application_id, QString release_id, QList<Repository *> repositories, GetApplicationInteractorListener *listener)
{
    Q_ASSERT(listener != nullptr);
    m_application_id = application_id;
    m_release_id = release_id;
    m_repositories = repositories;
    m_listener = listener;
}

GetApplicationInteractor::~GetApplicationInteractor()
{

}

void GetApplicationInteractor::execute()
{

    Release *release = nullptr;
    for (Repository *repository: m_repositories)
    {
        if (repository->contains(m_application_id, m_release_id))
        {
            release = repository->getRelease(m_application_id, m_release_id);
            break;
        }
     }

    if (release == nullptr)
    {
        QVariantMap data;
        data["app_id"] = QVariant::fromValue(release->app_id);
        data["release_id"] = release->id;

        data["arch"] = release->arch;
        data["autors"] = QVariant::fromValue(release->authors);
        data["description"] = release->description;
        data["download_link"] = release->download_link;
        data["download_size"] = release->download_size;
        data["icon_link"] = release->icon_link;
        data["licence"] = release->licence;
        data["name"] = release->name;
        data["screenshot_links"] = QVariant::fromValue(release->screenshot_links);

        m_listener->handleApplicationDetails(data);
    } else
        m_listener->handleNoApplicationFound();
}
