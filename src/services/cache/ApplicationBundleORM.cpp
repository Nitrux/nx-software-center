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
{
}

bool ApplicationBundleORM::applicationBundleTableExists() const
{
    QString checkTableExistsQueryTemplate(SQLITE_CHECK_TABLE_EXIST_QUERY);
    QSqlQuery query(checkTableExistsQueryTemplate.arg(APPLICATION_BUNDLES_TABLE_NAME), _database);
    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << "ApplicationBundleORM::applicationBundleTableExists" << error;

    bool hasData = query.next();
    return hasData && query.value(0).toString() == QString(APPLICATION_BUNDLES_TABLE_NAME);
}

void ApplicationBundleORM::createTable()
{
    QString queryTemplate =
        "CREATE TABLE %1 ("
        "APPLICATION_ID text NOT NULL"
        ", PATH text NOT NULL"
        ", SIZE integer"
        ", LAST_MODIFIED text"
        ", HASH_SUM_MD5 text"
        ", BUNDLE_TYPE integer"
        ", APPLICATION_DATA text NOT NULL"
        ", FOREIGN KEY(APPLICATION_ID) REFERENCES APPLICATION(ID) ON DELETE CASCADE"
        ")";

    QSqlQuery query(queryTemplate.arg(APPLICATION_BUNDLES_TABLE_NAME), _database);
    const auto &error = query.lastError();
    if (error.isValid())
        qWarning() << error;
}
void ApplicationBundleORM::init()
{
    if (!applicationBundleTableExists())
        createTable();
}

void ApplicationBundleORM::save(const ApplicationBundle &bundle)
{
    QSqlQuery query(_database);
    query.prepare("INSERT INTO " APPLICATION_BUNDLES_TABLE_NAME
                  " (APPLICATION_ID, PATH, SIZE, LAST_MODIFIED, HASH_SUM_MD5, BUNDLE_TYPE, APPLICATION_DATA) VALUES ("
                  "  :application_id"
                  ", :path"
                  ", :size"
                  ", :last_modified"
                  ", :hash_sum_md5"
                  ", :bundle_type"
                  ", :application_data"
                  ")");
    query.bindValue(":application_id", bundle.data.getId());
    query.bindValue(":path", bundle.path);
    query.bindValue(":size", bundle.size);
    query.bindValue(":last_modified", bundle.lastModified.toString(SQLITE_DATE_TIME_FORMAT));
    query.bindValue(":hash_sum_md5", bundle.hashSumMD5);
    query.bindValue(":bundle_type", bundle.bundleType);
    query.bindValue(":application_data", bundle.data.toJson());
    query.exec();

    const auto &error = query.lastError();
    if (error.isValid())
        qWarning() << "ApplicationBundleORM::save" << error;
}
QList<ApplicationBundle> ApplicationBundleORM::retrieveApplicationBundles(QString applicationId)
{
    QString queryTemplate = "SELECT APPLICATION_ID, PATH, SIZE, LAST_MODIFIED, HASH_SUM_MD5, BUNDLE_TYPE, APPLICATION_DATA FROM " APPLICATION_BUNDLES_TABLE_NAME
                            " WHERE APPLICATION_ID = \"%1\"";
    QSqlQuery query(queryTemplate.arg(applicationId));
    const auto &error = query.lastError();
    if (error.isValid())
        qWarning() << "ApplicationBundleORM::save" << error;

    QList<ApplicationBundle> results;
    while (query.next()) {
        ApplicationBundle bundle;
        bundle.path = query.value(1).toString();
        bundle.size = query.value(2).toLongLong();
        bundle.lastModified = QDateTime::fromString(query.value(3).toString(), SQLITE_DATE_TIME_FORMAT);
        bundle.hashSumMD5 = query.value(4).toByteArray();
        bundle.bundleType = static_cast<ApplicationBundle::Type>(query.value(5).toInt());
        bundle.data = ApplicationData::fromJson(query.value(6).toByteArray());

        results << bundle;
    }

    return results;
}
void ApplicationBundleORM::removeApplicationBundles(const QString &applicationId)
{
    QString queryTemplate = "DELETE FROM " APPLICATION_BUNDLES_TABLE_NAME " WHERE APPLICATION_ID='%1'";
    QSqlQuery query(queryTemplate.arg(applicationId));
    const auto &error = query.lastError();
    if (error.isValid())
        qWarning() << "ApplicationBundleORM::save" << error;
}
