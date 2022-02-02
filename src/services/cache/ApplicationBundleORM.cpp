#include "ApplicationBundleORM.h"

// library
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

// local
#include "../ApplicationBundle.h"
#include "SqliteUtils.h"

ApplicationBundleORM::ApplicationBundleORM(const QSqlDatabase &database)
    : _ready(false)
    , _database(database)
    , _tableName("ApplicationBundle")
{
    createTable();
    _ready = tableExists();
}

void ApplicationBundleORM::createOrUpdate(const ApplicationBundle &bundle)
{
    if (_ready) {
        // add entry
    } else {
        qWarning() << "Unable to store application bundle data: cache not initialized properly.";
    }
}
bool ApplicationBundleORM::tableExists() const
{
    QString queryTemplate(SQLITE_CHECK_TABLE_EXIST_QUERY);
    QSqlQuery query(queryTemplate.arg(_tableName), _database);
    if (!query.exec())
        qWarning() << query.lastError();

    return query.size() > 0;
}
void ApplicationBundleORM::createTable()
{
    QString queryTemplate =
        "CREATE TABLE %1 ("
        "APPLICATION_ID text NOT NULL"
        ", PATH text NOT NULL"
        ", SIZE integer"
        ", LAST_MODOFIED text"
        ", HASH_SUM_MD5 text"
        ", BUNDLE_TYPE integer"
        //            ", FOREIGN KEY(APPLICATION_ID) REFERENCES APPLICATION_DATA(ID) ON DELETE CASCADE"
        ")";

    QSqlQuery query(queryTemplate.arg(_tableName), _database);
    qWarning() << query.lastError();
}
bool ApplicationBundleORM::isReady() const
{
    return false;
}
