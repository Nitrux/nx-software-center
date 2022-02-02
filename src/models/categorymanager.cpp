#include "categorymanager.h"

CategoryManager::CategoryManager()
    : m_appimagehubStore(new AppImageHubStore(this))
    , m_apprepoStore(new AppRepoStore("https://apprepo.de/rest/api"))
    , m_response(new CategoryResponseDTO())
{

}

void CategoryManager::getCategories() {
    m_response->categories.clear();

    // Invoke appimagehub api
    m_appimagehubStore->getCategories();

    connect(m_appimagehubStore, &Store::categoriesResponseReady, [=](CategoryResponseDTO *appimagehubResponse) {
        // Invoke apprepo api
        m_apprepoStore->getGroups(appimagehubResponse);

        connect(m_apprepoStore, &AppRepoStore::groupsResponseReady, [=](CategoryResponseDTO *apprepoResponse) {
            m_response->categories.append(apprepoResponse->categories);

            emit categoriesResponseReady(m_response);
        });
    });
}