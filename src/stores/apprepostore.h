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

class AppRepoStore : public QObject {
    Q_OBJECT

    public:
        enum SearchPackage {
            ALL,
            BY_SLUG,
            BY_GROUP
        };

        AppRepoStore(QString apiBaseUrl);

        const QString name();
        
        void getGroups();
        void getPackages(SearchPackage criteria, QString value = "");

        Q_SIGNAL void groupsResponseReady(QList<AppRepoGroupResponseDTO *> response);
        Q_SIGNAL void packagesResponseReady(QList<AppRepoPackageResponseDTO *> response);
        Q_SIGNAL void error(QNetworkReply::NetworkError error);
    private:
        QString API_BASEURL;
        QString API_GROUPS_URL;
        QString API_PACKAGES_URL;
        QString API_PACKAGES_BY_SLUG_URL;
        QString API_PACKAGES_BY_GROUP_URL;

        void parseGetGroupsResponseAndReply(QNetworkReply *reply);
        void parseGetPackagesResponseAndReply(QNetworkReply *reply);

        AppRepoPackageResponseDTO *createPackageResponseDTO(QJsonObject obj);
};