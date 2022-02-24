#include "storemanagerservice.h"

StoreManagerService::StoreManagerService(QObject *parent)
    : QObject(parent) {

}

void StoreManagerService::getCategories() {
    initStoreManagers();

    _appimagehubStoreManager->getCategories();
}

void StoreManagerService::getApplications(QList<QString> categoriesFilter, QString nameFilter,
                                          Store::SORT_MODE sortMode, QString page,
                                          QString pageSize, QList<QString> tags,
                                          Store::Arch arch, Category *category) {
    initStoreManagers();

    if ( category->categoryStore == Category::CategoryStore::APPIMAGEHUB ) {
        // Invoke appimagehub api
        _appimagehubStoreManager->setApplicationSearchFilter(categoriesFilter, nameFilter, sortMode, page, pageSize, tags, arch);
        _appimagehubStoreManager->getApplications();
    } else if ( category->categoryStore == Category::CategoryStore::APPREPO ) {
        // Invoke apprepo api
        if ( page=="0" || page=="" ) {
            _apprepoStoreManager->setPackageFilter(category->id.toInt(), nameFilter);
            _apprepoStoreManager->getApplications();
        }
    } else {
        // Invoke appimagehub api
        _appimagehubStoreManager->setApplicationSearchFilter(categoriesFilter, nameFilter, sortMode, page, pageSize, tags, arch);
        _appimagehubStoreManager->getApplications();

        if ( nameFilter.size() > 0 ) {
            // Invoke apprepo api
            if ( page=="0" || page=="" ) {
                _apprepoStoreManager->setPackageFilter(category->id.toInt(), nameFilter);
                _apprepoStoreManager->getApplications();
            }
        }
    }
}

void StoreManagerService::initStoreManagers() {
    // Initialize the appimagehub store manager
    if ( _appimagehubStoreManager == nullptr ) {
        _appimagehubStoreManager = new AppimagehubStoreManager(this);

        connect(_appimagehubStoreManager, &AppimagehubStoreManager::categoriesResponseReady, this, &StoreManagerService::getApprepoCategories);
        connect(_appimagehubStoreManager, &AppimagehubStoreManager::applicationsResponseReady, this, &StoreManagerService::applicationsResponseReady);
    }

    // Initialize the apprepo store manager
    if ( _apprepoStoreManager == nullptr ) {
        _apprepoStoreManager = new ApprepoStoreManager(this);

        connect(_apprepoStoreManager, &ApprepoStoreManager::categoriesResponseReady, this, &StoreManagerService::categoriesResponseReady);
        connect(_apprepoStoreManager, &ApprepoStoreManager::applicationsResponseReady, this, &StoreManagerService::applicationsResponseReady);
    }
}

void StoreManagerService::getApprepoCategories(CategoryResponseDTO *appimagehubResponse) {
    initStoreManagers();

    // Set the app image hub category so that the apprepo categories can be added as subcategories
    _apprepoStoreManager->setCategoryFilter(appimagehubResponse);
        
    _apprepoStoreManager->getCategories();
}