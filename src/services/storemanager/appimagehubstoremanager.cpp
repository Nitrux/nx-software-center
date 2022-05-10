#include "appimagehubstoremanager.h"

AppimagehubStoreManager::AppimagehubStoreManager(QObject *parent)
    : StoreManager(parent) {
}

void AppimagehubStoreManager::getCategories() {
    initStore();

    // Invoke appimagehub api
    m_appimagehubStore->getCategories();
}

void AppimagehubStoreManager::getApplications() {
    initStore();

    // Invoke appimagehub api
    m_appimagehubStore->getApplicationsByArch(_categoriesFilter, _nameFilter, _sortMode, _page, _pageSize, _tags, _arch);
}

void AppimagehubStoreManager::initStore() {
    if ( m_appimagehubStore == nullptr ) {
        m_appimagehubStore = new AppImageHubStore(this);

        connect(m_appimagehubStore,  &Store::categoriesResponseReady, this, &AppimagehubStoreManager::categoriesResponseReady);
        connect(m_appimagehubStore,  &Store::applicationsResponseReady, this, &AppimagehubStoreManager::applicationsResponseReady);
    }
}

void AppimagehubStoreManager::setApplicationSearchFilter(QList<QString> categoriesFilter,
                                                         QString nameFilter,
                                                         Store::SORT_MODE sortMode,
                                                         QString page,
                                                         QString pageSize,
                                                         QList<QString> tags,
                                                         Store::Arch arch) {
    
    _categoriesFilter = categoriesFilter;
    _nameFilter = nameFilter;
    _sortMode = sortMode;
    _page = page;
    _pageSize = pageSize;
    _tags = tags;
    _arch = arch;
}