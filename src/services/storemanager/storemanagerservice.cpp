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
                                          QString pageSize, QList<QString> tags, QString user,
                                          Store::Arch arch, Category *category) {
    initStoreManagers();

    // Invoke appimagehub api
    _appimagehubStoreManager->setApplicationSearchFilter(categoriesFilter, nameFilter, sortMode, page, pageSize, tags, user, arch);
    _appimagehubStoreManager->getApplications();
}

void StoreManagerService::initStoreManagers() {
    // Initialize the appimagehub store manager
    if ( _appimagehubStoreManager == nullptr ) {
        _appimagehubStoreManager = new AppimagehubStoreManager(this);

        connect(_appimagehubStoreManager, &AppimagehubStoreManager::categoriesResponseReady, this, &StoreManagerService::categoriesResponseReady);
        connect(_appimagehubStoreManager, &AppimagehubStoreManager::applicationsResponseReady, this, &StoreManagerService::applicationsResponseReady);
    }
}
