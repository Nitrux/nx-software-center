#pragma once
// system

// libraries
#include<QJsonArray>
#include<QJsonDocument>

// local
#include "../../utils/nx.h"
#include "../services/ApplicationData.h"
#include "BundlesDBHelper.h"
#include "DBHelper.h"

class AppsDBHelper : public DBHelper {
    public:
        AppsDBHelper();

        QList<ApplicationData> getApps();
        QMap<QString, ApplicationData> getAppsMap();
        ApplicationData *getAppById(QString id);
        ApplicationData *getAppByName(QString name);
        
        Q_SLOT bool saveOrUpdateApp(const ApplicationData &app);
        Q_SLOT bool deleteApp(const ApplicationData &app);

    private:
        QStringList toStringList(QList<QUrl> list);
        QList<QUrl> snapshotsFromJsonArray(QJsonArray data);
        QStringList categoriesFromJsonArray(QJsonArray data);

        BundlesDBHelper *_bundlesDBHelper;

        const QString DATE_FORMAT = "yyyy-MM-dd HH:mm:ss.zzz";

        const QString QUERY_INSERT_APPLICATION_DATA = "INSERT INTO APPLICATION_DATA (ID, VERSION, NAME, ICON, DESCRIPTION, SNAPSHOTS, CATEGORIES) values (:id, :version, :name, :icon, :description, :snapshots, :categories) ";
        const QString QUERY_UPDATE_APPLICATION_DATA = "UPDATE APPLICATION_DATA SET VERSION=:version, NAME=:name, ICON=:icon, DESCRIPTION=:description, SNAPSHOTS=:snapshots, CATEGORIES=:categories WHERE ID=:id ";
        const QString QUERY_DELETE_APPLICATION_DATA = "DELETE FROM APPLICATION_DATA where ID=(:appId)";

        const QString QUERY_SELECT_APPLICATION_DATA = "SELECT * FROM APPLICATION_DATA";
        const QString QUERY_SELECT_APPLICATION_DATA_BY_ID = "SELECT * FROM APPLICATION_DATA where ID=(:appId)";
        const QString QUERY_SELECT_APPLICATION_DATA_BY_NAME = "SELECT * FROM APPLICATION_DATA where NAME=(:appName)";
};
