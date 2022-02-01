#include "applicationmanager.h"

ApplicationManager::ApplicationManager()
    : m_appimagehubStore(new AppImageHubStore(this))
    , m_apprepoStore(new AppRepoStore("https://apprepo.de/rest/api"))
    , m_response(new ApplicationResponseDTO())
{

}

void ApplicationManager::getApplications(QList<QString> categoriesFilter, QString nameFilter,
                                             Store::SORT_MODE sortMode, QString page,
                                             QString pageSize, QList<QString> tags,
                                             Store::Arch arch, Category *category) {
    m_response->applications.clear();

    if ( category->categoryStore == Category::CategoryStore::APPIMAGEHUB ) {
        // Invoke appimagehub api
        m_appimagehubStore->getApplicationsByArch(categoriesFilter, nameFilter, sortMode, page, pageSize, tags, arch);
    } else if ( category->categoryStore == Category::CategoryStore::APPREPO ) {
        // Invoke apprepo api
        if ( page=="0" || page=="" ) {
            m_apprepoStore->getPackagesByGroup(category->id.toInt());
        }
    } else {
        // Invoke appimagehub api
        m_appimagehubStore->getApplicationsByArch(categoriesFilter, nameFilter, sortMode, page, pageSize, tags, arch);

        // Invoke apprepo api
        m_apprepoStore->getPackages();
    }

    connect(m_appimagehubStore, &Store::applicationsResponseReady, [=](ApplicationResponseDTO *appimagehubResponse) {
        m_response->applications.append(appimagehubResponse->applications);

        emit applicationsResponseReady(m_response);
    });

    connect(m_apprepoStore, &AppRepoStore::packagesResponseReady, [=](ApplicationResponseDTO *apprepoResponse) {
        // Since apprepo api does not have option to fetch apps by page and page size
        // so to limit the total number of apps from apprepo only a part is displayed.
        // For apps fetched by categories all apps are appended to the list.
        if ( category->categoryStore == Category::CategoryStore::APPREPO ) {
            m_response->applications.append(apprepoResponse->applications);
        } else {
            m_response->applications.append(apprepoResponse->applications.mid(0, 10));
        }

        emit applicationsResponseReady(m_response);
    });
}