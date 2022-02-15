#include "apprepostoremanager.h"

ApprepoStoreManager::ApprepoStoreManager(QObject *parent)
    : StoreManager(parent) {

}

void ApprepoStoreManager::setTopCategory(CategoryResponseDTO *topCategory) {
    _topCategory = topCategory;
}

void ApprepoStoreManager::setPackageGroupFilter(int packageGroup) {
    _packageGroup = packageGroup;
}

void ApprepoStoreManager::getCategories() {
    initStore();

    // Invoke apprepo api
    m_apprepoStore->getGroups(_topCategory);
}

void ApprepoStoreManager::getApplications() {
    initStore();

    // Invoke apprepo api
    m_apprepoStore->getPackagesByGroup(_packageGroup);
}

void ApprepoStoreManager::initStore() {
    if ( m_apprepoStore == nullptr ) {
        m_apprepoStore = new AppRepoStore("https://apprepo.de/rest/api");

        connect(m_apprepoStore, &AppRepoStore::groupsResponseReady, this, &ApprepoStoreManager::categoriesResponseReady);
        connect(m_apprepoStore, &AppRepoStore::packagesResponseReady, this, &ApprepoStoreManager::applicationsResponseReady);
    }
}