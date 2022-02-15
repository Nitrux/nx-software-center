#pragma once

#include "storemanager.h"
#include "../../stores/apprepostore.h"
#include "../../stores/ResponseDTO/categoryresponsedto.h"

class ApprepoStoreManager : public StoreManager {
    Q_OBJECT

    public:
        ApprepoStoreManager(QObject *parent = nullptr);

        void getCategories() override final;
        void getApplications() override final;

        void setTopCategory(CategoryResponseDTO *appimagehubCategory);
        void setPackageGroupFilter(int packageGroup);
    private:
        void initStore();

        AppRepoStore *m_apprepoStore = nullptr;
        CategoryResponseDTO *_topCategory;
        int _packageGroup;
};