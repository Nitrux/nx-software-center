#pragma once

#include "storemanager.h"
#include "../../stores/store.h"
#include "../../stores/appimagehubstore.h"

class AppimagehubStoreManager : public StoreManager {
    Q_OBJECT

    public:
        AppimagehubStoreManager(QObject *parent = nullptr);

        void getCategories() override final;
        void getApplications() override final;
        
    private:
        void initStore();
        
        Store *m_appimagehubStore = nullptr;
};