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
    , _bundlesORM(_database)

{
    ensureCacheDbParentDirExists();

    _database.setDatabaseName(_path);
    bool openSucceed = _database.open();
    if (!openSucceed)
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
    return _database.isOpen() && _bundlesORM.isReady();
}
void CacheService::saveApplication(const Application &application)
{
}
ApplicationsList CacheService::listApplications()
{
    return ApplicationsList();
}
void CacheService::deleteApplication(const Application &application)
{
}
