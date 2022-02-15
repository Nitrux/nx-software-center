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
}

void AppimagehubStoreManager::initStore() {
    if ( m_appimagehubStore == nullptr ) {
        m_appimagehubStore = new AppImageHubStore(this);

        connect(m_appimagehubStore,  &Store::categoriesResponseReady, this, &AppimagehubStoreManager::categoriesResponseReady);
    }
}