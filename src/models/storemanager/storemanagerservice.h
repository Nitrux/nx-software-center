#pragma once

#include "apprepostoremanager.h"
#include "appimagehubstoremanager.h"

class StoreManagerService : public QObject {
    Q_OBJECT

    public:
        StoreManagerService(QObject *parent = nullptr);

        void getCategories();

        Q_SIGNAL void categoriesResponseReady(CategoryResponseDTO *response);

    private:
        void initStoreManagers();
        void getApprepoCategories(CategoryResponseDTO *response);

        ApprepoStoreManager *_apprepoStoreManager = nullptr;
        AppimagehubStoreManager *_appimagehubStoreManager = nullptr;
};