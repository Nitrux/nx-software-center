#include "ApplicationORM.h"

// libraries
#include <QDebug>
#include <QJsonDocument>
#include <QSqlError>
#include <QSqlQuery>

// local
#include "ApplicationBundleORM.h"
#include "SqliteUtils.h"

ApplicationORM::ApplicationORM(const QSqlDatabase &database)
    : _database(database)
{
}

ApplicationORM::~ApplicationORM()
{
    //    delete _bundleORM;
    //    delete _dataORM;
}

void ApplicationORM::init()
{
    if (!applicationTableExists())
        createApplicationTable();
}

void ApplicationORM::createApplicationTable()
{
    qDebug() << "ApplicationORM: Creating table";
    QSqlQuery query("CREATE TABLE " APPLICATIONS_TABLE_NAME
                    " ( "
                    "id text NOT NULL"
                    ", mainBundleIdx int"
                    ", data text"
                    ");",
                    _database);
    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << __FUNCTION__ << error;
}
bool ApplicationORM::applicationTableExists() const
{
    QString checkTableExistsQueryTemplate(SQLITE_CHECK_TABLE_EXIST_QUERY);
    QSqlQuery query(checkTableExistsQueryTemplate.arg(APPLICATIONS_TABLE_NAME), _database);
    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << __FUNCTION__ << error;

    bool hasData = query.next();
    return hasData && query.value(0).toString() == QString(APPLICATIONS_TABLE_NAME);
}

void ApplicationORM::create(const Application &application) const
{
    QSqlQuery query(_database);
    query.prepare("INSERT INTO " APPLICATIONS_TABLE_NAME " (id, mainBundleIdx, data) VALUES (:id, :mainBundleIdx, :data);");
    query.bindValue(":id", application.getId());
    query.bindValue(":mainBundleIdx", application.getMainBundleIndex());

    const auto &appdata = application.getData();
    query.bindValue(":data", appdata.toJson());

    query.exec();
    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << __FUNCTION__ << error;
}

ApplicationsList ApplicationORM::retrieve()
{
    ApplicationsList results;
    QSqlQuery query("SELECT mainBundleIdx, data FROM " APPLICATIONS_TABLE_NAME ";", _database);
    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << __FUNCTION__ << error;

    while (query.next()) {
        const auto &applicationDataJson = query.value(1).toByteArray();
        ApplicationData appData = ApplicationData::fromJson(applicationDataJson);
        results << Application(appData);
    }
    return results;
}
Application ApplicationORM::retrieveById(const QString &id)
{
    QString queryTemplate("SELECT id, data FROM %1 WHERE id = '%2';");
    QSqlQuery query(queryTemplate.arg(APPLICATIONS_TABLE_NAME, id), _database);
    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << __FUNCTION__ << error;

    while (query.next()) {
        const auto &applicationDataJson = query.value(1).toByteArray();
        ApplicationData appData = ApplicationData::fromJson(applicationDataJson);
        return Application(appData);
    }

    return {};
}
void ApplicationORM::update(const Application &application)
{
    QString queryTemplate = "UPDATE " APPLICATIONS_TABLE_NAME " SET data = '%1' WHERE id = '%2'";
    const auto &dataJson = application.getData().toJson();
    QSqlQuery query(queryTemplate.arg(dataJson, application.getId()), _database);

    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << __FUNCTION__ << error;
}
void ApplicationORM::removeById(const QString &id)
{
    QString queryTemplate = "DELETE FROM " APPLICATIONS_TABLE_NAME " WHERE id = '%1';";
    QSqlQuery query(queryTemplate.arg(id), _database);

    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << __FUNCTION__ << error;
}
