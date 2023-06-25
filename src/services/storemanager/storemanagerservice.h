#pragma once

#include "appimagehubstoremanager.h"
#include "../../stores/store.h"
#include "../../stores/ResponseDTO/category.h"

class StoreManagerService : public QObject {
    Q_OBJECT

    public:
        StoreManagerService(QObject *parent = nullptr);

        void getCategories();
        void getApplications(QList<QString> categoriesFilter, QString nameFilter = "",
                             Store::SORT_MODE sortMode = Store::SORT_MODE::MODE_NEWEST, QString page = "",
                             QString pageSize = "", QList<QString> tags = {}, QString user = "",
                             Store::Arch arch = Store::Arch::all, Category *category = nullptr);

        Q_SIGNAL void categoriesResponseReady(CategoryResponseDTO *response);
        Q_SIGNAL void applicationsResponseReady(ApplicationResponseDTO *response);

    private:
        void initStoreManagers();

        AppimagehubStoreManager *_appimagehubStoreManager = nullptr;
};
