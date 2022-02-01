#pragma once

#include "../stores/store.h"
#include "../stores/apprepostore.h"
#include "../stores/appimagehubstore.h"
#include "../stores/ResponseDTO/category.h"

class ApplicationManager : public QObject {
    Q_OBJECT

    public:
        ApplicationManager();

        void getApplications(QList<QString> categoriesFilter, QString nameFilter = "",
                             Store::SORT_MODE sortMode = Store::SORT_MODE::MODE_NEWEST, QString page = "",
                             QString pageSize = "", QList<QString> tags = {},
                             Store::Arch arch = Store::Arch::all, Category *category = nullptr);

        Q_SIGNAL void applicationsResponseReady(ApplicationResponseDTO *response);
        Q_SIGNAL void error(QNetworkReply::NetworkError error);

    private:
        Store *m_appimagehubStore;
        AppRepoStore *m_apprepoStore;

        ApplicationResponseDTO *m_response;
};