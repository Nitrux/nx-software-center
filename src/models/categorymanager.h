#pragma once

#include "../stores/apprepostore.h"
#include "../stores/appimagehubstore.h"

class CategoryManager : public QObject {
    Q_OBJECT

    public:
        CategoryManager();

        void getCategories();

        Q_SIGNAL void categoriesResponseReady(CategoryResponseDTO *response);
        Q_SIGNAL void error(QNetworkReply::NetworkError error);

    private:
        Store *m_appimagehubStore;
        AppRepoStore *m_apprepoStore;

        CategoryResponseDTO *m_response;
};