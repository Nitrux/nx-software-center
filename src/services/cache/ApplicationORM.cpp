#include "ApplicationORM.h"

// libraries
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

// local
#include "ApplicationBundleORM.h"
#include "ApplicationDataORM.h"
#include "SqliteUtils.h"

ApplicationORM::ApplicationORM(const QSqlDatabase &database)
    : _database(database)
//    , _dataORM(new ApplicationDataORM(database))
//    , _bundleORM(new ApplicationBundleORM(database))
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
                    ", data text"
                    ");",
                    _database);
    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << "ApplicationORM:" << error;
}
bool ApplicationORM::applicationTableExists() const
{
    QString checkTableExistsQueryTemplate(SQLITE_CHECK_TABLE_EXIST_QUERY);
    QSqlQuery query(checkTableExistsQueryTemplate.arg(APPLICATIONS_TABLE_NAME), _database);
    const auto &error = query.lastError();
    if (error.isValid())
        qDebug() << "ApplicationORM:" << error;

    query.next();
    return query.value(0).toString() == QString(APPLICATIONS_TABLE_NAME);
}
