#include "storemanagerservice.h"

StoreManagerService::StoreManagerService(QObject *parent)
    : QObject(parent) {

}

void StoreManagerService::getCategories() {
    initStoreManagers();

    _appimagehubStoreManager->getCategories();
}

void StoreManagerService::getApprepoCategories(CategoryResponseDTO *appimagehubResponse) {
    initStoreManagers();

    // Set the app image hub category so that the apprepo categories can be added as subcategories
    _apprepoStoreManager->setCategory(appimagehubResponse);
        
    _apprepoStoreManager->getCategories();
}

void StoreManagerService::initStoreManagers() {
    // Initialize the appimagehub store manager
    if ( _appimagehubStoreManager == nullptr ) {
        _appimagehubStoreManager = new AppimagehubStoreManager(this);

        connect(_appimagehubStoreManager, &AppimagehubStoreManager::categoriesResponseReady, this, &StoreManagerService::getApprepoCategories);
    }

    // Initialize the apprepo store manager
    if ( _apprepoStoreManager == nullptr ) {
        _apprepoStoreManager = new ApprepoStoreManager(this);

        connect(_apprepoStoreManager, &ApprepoStoreManager::categoriesResponseReady, this, &StoreManagerService::categoriesResponseReady);
    }
}