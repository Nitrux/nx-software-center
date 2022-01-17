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

class AppsDBHelper : public QObject {
    Q_OBJECT

    public:
        AppsDBHelper();
        ~AppsDBHelper();

        QList<ApplicationData> getApps();
        QMap<QString, ApplicationData> getAppsMap();
        ApplicationData *getAppById(QString id);
        ApplicationData *getAppByName(QString name);
        
        Q_SLOT bool saveOrUpdateApp(const ApplicationData &app);
        Q_SLOT bool deleteApp(const ApplicationData &app);

    private:
        
        void initDB();
        void prepareAppsDB();
        void openDB(const QString &name, const QString &appDBPath);

        QList<ApplicationBundle> getBundlesByApp(QString id);
        bool saveOrUpdateBundles(const QString id, const QList<ApplicationBundle> &bundles);
        bool deleteBundles(const QString id, const QList<ApplicationBundle> &bundles);

        QStringList toStringList(QList<QUrl> list);
        QList<QUrl> snapshotsFromJsonArray(QJsonArray data);
        QStringList categoriesFromJsonArray(QJsonArray data);

        static AppsDBHelper *appsDBHelper;
        QSqlDatabase _appsDB;

        const QString DATE_FORMAT = "yyyy-MM-dd HH:mm:ss.zzz";

        const QString QUERY_CREATE_APPLICATION_DATA = "CREATE TABLE APPLICATION_DATA (ID text NOT NULL PRIMARY KEY, VERSION text, NAME text NOT NULL, ICON text, DESCRIPTION text, SNAPSHOTS text, CATEGORIES text) ";
        const QString QUERY_INSERT_APPLICATION_DATA = "INSERT INTO APPLICATION_DATA (ID, VERSION, NAME, ICON, DESCRIPTION, SNAPSHOTS, CATEGORIES) values (:id, :version, :name, :icon, :description, :snapshots, :categories) ";
        const QString QUERY_UPDATE_APPLICATION_DATA = "UPDATE APPLICATION_DATA SET VERSION=:version, NAME=:name, ICON=:icon, DESCRIPTION=:description, SNAPSHOTS=:snapshots, CATEGORIES=:categories WHERE ID=:id ";
        const QString QUERY_DELETE_APPLICATION_DATA = "DELETE FROM APPLICATION_DATA where ID=(:appId)";

        const QString QUERY_CREATE_APPLICATION_BUNDLE = "CREATE TABLE APPLICATION_BUNDLE (APPLICATION_ID text NOT NULL, PATH text, SIZE integer, LAST_MODOFIED text, HASH_SUM_MD5 text, BUNDLE_TYPE integer, FOREIGN KEY(APPLICATION_ID) REFERENCES APPLICATION_DATA(ID) ON DELETE CASCADE) ";
        const QString QUERY_INSERT_APPLICATION_BUNDLE = "INSERT INTO APPLICATION_BUNDLE (APPLICATION_ID, PATH, SIZE, LAST_MODOFIED, HASH_SUM_MD5, BUNDLE_TYPE) values (:application_id, :path, :size, :last_modified, :hash_sum_md5, :bundle_type) ";
        const QString QUERY_UPDATE_APPLICATION_BUNDLE = "UPDATE APPLICATION_BUNDLE SET SIZE=:size, LAST_MODOFIED=:last_modified, HASH_SUM_MD5=:hash_sum_md5, BUNDLE_TYPE=:bundle_type WHERE APPLICATION_ID=:application_id and PATH=:path ";
        const QString QUERY_DELETE_APPLICATION_BUNDLE = "DELETE FROM APPLICATION_BUNDLE where APPLICATION_ID=? and PATH=?";

        const QString QUERY_SELECT_APPLICATION_DATA = "SELECT * FROM APPLICATION_DATA";
        const QString QUERY_SELECT_APPLICATION_DATA_BY_ID = "SELECT * FROM APPLICATION_DATA where ID=(:appId)";
        const QString QUERY_SELECT_APPLICATION_DATA_BY_NAME = "SELECT * FROM APPLICATION_DATA where NAME=(:appName)";

        const QString QUERY_SELECT_APPLICATION_BUNDLES_BY_ID = "SELECT * FROM APPLICATION_BUNDLE where APPLICATION_ID=(:application_id)";
};
