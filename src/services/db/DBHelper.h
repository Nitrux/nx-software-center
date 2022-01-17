#pragma once
// system

// libraries
#include<QDebug>
#include<QDir>
#include<QUuid>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

// local
#include "../../utils/nx.h"

class DBHelper : public QObject {
    Q_OBJECT

    public:
        DBHelper();
        ~DBHelper();

        QSqlDatabase _appsDB;
        void initDB();
        void prepareAppsDB();
        void openDB(const QString &name, const QString &appDBPath);

    private:
        const QString QUERY_CREATE_APPLICATION_DATA = "CREATE TABLE APPLICATION_DATA (ID text NOT NULL PRIMARY KEY, VERSION text, NAME text NOT NULL, ICON text, DESCRIPTION text, SNAPSHOTS text, CATEGORIES text) ";
        const QString QUERY_CREATE_APPLICATION_BUNDLE = "CREATE TABLE APPLICATION_BUNDLE (APPLICATION_ID text NOT NULL, PATH text, SIZE integer, LAST_MODOFIED text, HASH_SUM_MD5 text, BUNDLE_TYPE integer, FOREIGN KEY(APPLICATION_ID) REFERENCES APPLICATION_DATA(ID) ON DELETE CASCADE) ";
};