#pragma once

#include "storemanager.h"
#include "../../stores/store.h"
#include "../../stores/appimagehubstore.h"
#include "../../stores/store.h"
#include "../../stores/ResponseDTO/category.h"

class AppimagehubStoreManager : public StoreManager {
    Q_OBJECT

    public:
        AppimagehubStoreManager(QObject *parent = nullptr);

        void getCategories() override final;
        void getApplications() override final;
        
        void setApplicationSearchFilter(QList<QString> categoriesFilter,
                                        QString nameFilter,
                                        Store::SORT_MODE sortMode,
                                        QString page,
                                        QString pageSize,
                                        QList<QString> tags, QString user,
                                        Store::Arch arch);
    private:
        void initStore();
        
        Store *m_appimagehubStore = nullptr;

        // Application search filters
        QList<QString> _categoriesFilter;
        QString _nameFilter;
        Store::SORT_MODE _sortMode;
        QString _page;
        QString _pageSize;
        QList<QString> _tags;
        QString _user;
        Store::Arch _arch;
};
