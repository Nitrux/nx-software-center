#include "apprepostoremanager.h"

ApprepoStoreManager::ApprepoStoreManager(QObject *parent)
    : StoreManager(parent) {

}

void ApprepoStoreManager::setCategory(CategoryResponseDTO *appimagehubCategory) {
    _appimagehubCategory = appimagehubCategory;
}

void ApprepoStoreManager::getCategories() {
    initStore();

    // Invoke apprepo api
    m_apprepoStore->getGroups(_appimagehubCategory);
}

void ApprepoStoreManager::getApplications() {
    initStore();
}

void ApprepoStoreManager::initStore() {
    if ( m_apprepoStore == nullptr ) {
        m_apprepoStore = new AppRepoStore("https://apprepo.de/rest/api");

        connect(m_apprepoStore, &AppRepoStore::groupsResponseReady, this, &ApprepoStoreManager::categoriesResponseReady);
    }
}