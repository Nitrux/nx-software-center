#include "CacheService.h"

// libraries
#include <QDebug>
#include <QDir>

// local
#include "ApplicationBundleORM.h"

CacheService::CacheService(QString path)
    : _path(std::move(path))
    , _database(QSqlDatabase::addDatabase("QSQLITE"))
    , _applicationsORM(_database)
{
    ensureCacheDbParentDirExists();

    _database.setDatabaseName(_path);
    bool openSucceed = _database.open();
    if (openSucceed) {
        _applicationsORM.init();
    } else
        qWarning() << "Unable to create/open cache database at:" << _path;
}

void CacheService::ensureCacheDbParentDirExists() const
{
    QDir appDataLocation_dir(_path);
    appDataLocation_dir.cdUp();

    if (!appDataLocation_dir.exists())
        appDataLocation_dir.mkpath(".");
}

CacheService::~CacheService()
{
    if (this->_database.isOpen()) {
        this->_database.close();
    }
}
bool CacheService::isOperational() const
{
    return _database.isOpen() && _applicationsORM.applicationTableExists();
}
void CacheService::saveApplication(const Application &application)
{
    _applicationsORM.createOrUpdateApplication(application);
}
ApplicationsList CacheService::listApplications()
{
    return _applicationsORM.listApplications();
}
void CacheService::deleteApplication(const Application &application)
{
}
