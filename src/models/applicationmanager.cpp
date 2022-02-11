#include "applicationmanager.h"

ApplicationManager::ApplicationManager(QObject *parent)
    : QObject(parent)
    , m_appimagehubStore(new AppImageHubStore(this))
    , m_apprepoStore(new AppRepoStore("https://apprepo.de/rest/api", this))
{

    connect(m_appimagehubStore, &AppImageHubStore::applicationsResponseReady, [=](ApplicationResponseDTO *appimagehubResponse) {

        emit applicationsResponseReady(appimagehubResponse);
    });

    connect(m_apprepoStore, &AppRepoStore::packagesResponseReady, [=](ApplicationResponseDTO *apprepoResponse) {
        emit applicationsResponseReady(apprepoResponse);
    });
}

void ApplicationManager::getApplications(QList<QString> categoriesFilter, QString nameFilter,
                                         Store::SORT_MODE sortMode, QString page,
                                         QString pageSize, QList<QString> tags,
                                         Store::Arch arch, Category *category) {

    if ( category->categoryStore == Category::CategoryStore::APPIMAGEHUB ) {
        // Invoke appimagehub api
        m_appimagehubStore->getApplicationsByArch(categoriesFilter, nameFilter, sortMode, page, pageSize, tags, arch);

        // On clicking a top level category (appimagehub category), fetch the apps from apprepo sub-categories as well
        // which is there under the clicked top level category. So that all apps from appimagehub and apprepo are shown
        // foreach (Category *item, category->categories) {
        //     if ( item->categoryStore == Category::CategoryStore::APPREPO ) {
        //         if ( page=="0" || page=="" ) {
        //             m_apprepoStore->getPackagesByGroup(item->id.toInt());
        //         }
        //     }
        // }
    } else if ( category->categoryStore == Category::CategoryStore::APPREPO ) {
        // Invoke apprepo api
        if ( page=="0" || page=="" ) {
            m_apprepoStore->getPackagesByGroup(category->id.toInt());
        }
    } else {
        // Invoke appimagehub api
        m_appimagehubStore->getApplicationsByArch(categoriesFilter, nameFilter, sortMode, page, pageSize, tags, arch);

        // This is not required because no apprepo section is getting displayed in store home
        // Invoke apprepo api
        // m_apprepoStore->getPackages();
    }
}
