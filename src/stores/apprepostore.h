/**
 * Fetches the apps list from api url https://apprepo.de/rest/api
 */

#pragma once

// system

// libraries
#include <QList>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>

// local
#include "store.h"
#include "ResponseDTO/apprepo/apprepogroupresponsedto.h"
#include "ResponseDTO/apprepo/apprepopackageresponsedto.h"
#include "ResponseDTO/categoryresponsedto.h"
#include "ResponseDTO/category.h"
#include "ResponseDTO/applicationresponsedto.h"
#include "ResponseDTO/application.h"

class AppRepoStore : public QObject {
    Q_OBJECT

    public:

        AppRepoStore(QString apiBaseUrl);

        const QString name();
        
        void getGroups(CategoryResponseDTO *appimagehubResponse);
        void getPackages();
        void getPackagesBySlug(QString slug);
        void getPackagesByGroup(int group);

        Q_SIGNAL void groupsResponseReady(CategoryResponseDTO *response);
        Q_SIGNAL void packagesResponseReady(ApplicationResponseDTO *response);
        Q_SIGNAL void error(QNetworkReply::NetworkError error);
    private:
        enum SearchPackage {
            ALL,
            BY_SLUG,
            BY_GROUP
        };

        QString API_BASEURL;
        QString API_GROUPS_URL;
        QString API_PACKAGES_URL;
        QString API_PACKAGES_BY_SLUG_URL;
        QString API_PACKAGES_BY_GROUP_URL;

        const QList<QString> audioCategories = {"Audio editors", "Audio recording software", "Audio players"};
	    const QList<QString> educationCategories = {"Education", "Science", "eBook readers"};
        const QList<QString> graphicsCategories = {"Graphic", "Diagrams", "3D Editors", "Image viewers", "Multimedia"};
        const QList<QString> internetCategories = {"Browsers", "Internet", "Messengers", "Email", "Cloud storage"};
        const QList<QString> officeCategories = {"Office"};
        const QList<QString> programmingCategories = {"Development", "Database managers", "IDE", "API clients", "LaTeX editors"};
        const QList<QString> systemAndToolsCategories = {"Utilites", "Command line applications", "Disk Utility", "Text editors", "PDF toos", "Maps and navigation", "File managers", "3D Printing", "Scanning", "Screenshots", "RDP / VNC", "Notes", "Privacy"};
        const QList<QString> videoCategories = {"Video"};
        
        void getPackages(SearchPackage criteria, QString value = "");

        void parseGetGroupsResponseAndReply(QNetworkReply *reply, CategoryResponseDTO *appimagehubResponse);
        void parseGetPackagesResponseAndReply(QNetworkReply *reply);

        ApplicationResponseDTO *generatePackageResponse(QList<AppRepoPackageResponseDTO *> response);

        AppRepoPackageResponseDTO *createPackageResponseDTO(QJsonObject obj);

        void mapCategoryToSubcategory(CategoryResponseDTO *appimagehubResponse, QList<AppRepoGroupResponseDTO *> response);
        void populateCategoryToSubcategory(QString categoryType, CategoryResponseDTO *appimagehubResponse, AppRepoGroupResponseDTO *responseItem);
};