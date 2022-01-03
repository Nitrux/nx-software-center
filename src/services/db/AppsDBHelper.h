#pragma once
// system

// libraries
#include <QString>
#include<QDebug>
#include<QList>
#include<QDir>
#include<QUuid>
#include<QJsonArray>
#include<QJsonDocument>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

// local
#include "../../utils/nx.h"
#include "../services/ApplicationData.h"

class AppsDBHelper {
    public:
        static AppsDBHelper *getInstance();
        ~AppsDBHelper();

        QList<ApplicationData> getApps();
        ApplicationData *getAppById(QString id);
        ApplicationData *getAppByName(QString name);
        bool saveOrUpdateApp(ApplicationData *app);
        bool deleteApp(ApplicationData *app);

    private:
        AppsDBHelper();
        
        void initDB();
        void prepareAppsDB();
        void openDB(const QString &name, const QString &appDBPath);

        QStringList toStringList(QList<QUrl> list);
        QList<QUrl> snapshotsFromJsonArray(QJsonArray data);
        QStringList categoriesFromJsonArray(QJsonArray data);

        static AppsDBHelper *appsDBHelper;
        QSqlDatabase _appsDB;

        const QString QUERY_CREATE_APPLICATION_DATA = "CREATE TABLE APPLICATION_DATA (ID text NOT NULL PRIMARY KEY, VERSION text, NAME text NOT NULL, ICON text, DESCRIPTION text, SNAPSHOTS text, CATEGORIES text) ";
        const QString QUERY_INSERT_APPLICATION_DATA = "INSERT INTO APPLICATION_DATA (ID, VERSION, NAME, ICON, DESCRIPTION, SNAPSHOTS, CATEGORIES) values (:id, :version, :name, :icon, :description, :snapshots, :categories) ";
        const QString QUERY_UPDATE_APPLICATION_DATA = "UPDATE APPLICATION_DATA SET VERSION=:version, NAME=:name, ICON=:icon, DESCRIPTION=:description, SNAPSHOTS=:snapshots, CATEGORIES=:categories WHERE ID=:id ";
        const QString QUERY_DELETE_APPLICATION_DATA = "DELETE FROM APPLICATION_DATA where ID=(:appId)";

        const QString QUERY_SELECT_APPLICATION_DATA = "SELECT * FROM APPLICATION_DATA";
        const QString QUERY_SELECT_APPLICATION_DATA_BY_ID = "SELECT * FROM APPLICATION_DATA where ID=(:appId)";
        const QString QUERY_SELECT_APPLICATION_DATA_BY_NAME = "SELECT * FROM APPLICATION_DATA where NAME=(:appName)";
};
