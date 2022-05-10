#include "apprepostoremanager.h"

ApprepoStoreManager::ApprepoStoreManager(QObject *parent)
    : StoreManager(parent) {

}

void ApprepoStoreManager::setCategoryFilter(CategoryResponseDTO *topCategory) {
    _topCategory = topCategory;
}

void ApprepoStoreManager::setPackageFilter(int packageGroup, QString nameFilter) {
    _packageGroup = packageGroup;
    _nameFilter = nameFilter;
}

void ApprepoStoreManager::getCategories() {
    initStore();

    // Invoke apprepo api
    m_apprepoStore->getGroups(_topCategory);
}

void ApprepoStoreManager::getApplications() {
    initStore();

    // Invoke apprepo api
    if ( _nameFilter.size() > 0 ) {
        m_apprepoStore->searchPackages(_nameFilter);
    } else {
        m_apprepoStore->getPackagesByGroup(_packageGroup);
    }
}

void ApprepoStoreManager::initStore() {
    if ( m_apprepoStore == nullptr ) {
        m_apprepoStore = new AppRepoStore("https://apprepo.de/rest/api");

        connect(m_apprepoStore, &AppRepoStore::groupsResponseReady, this, &ApprepoStoreManager::categoriesResponseReady);
        connect(m_apprepoStore, &AppRepoStore::packagesResponseReady, this, &ApprepoStoreManager::applicationsResponseReady);
    }
}