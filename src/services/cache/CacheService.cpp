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
    , _applicationBundleORM(_database)
{
    ensureCacheDbParentDirExists();

    _database.setDatabaseName(_path);
    bool openSucceed = _database.open();
    if (openSucceed) {
        _applicationsORM.init();
        _applicationBundleORM.init();
    } else
        qWarning() << "Unable to create/open cache database at:" << _path;
}

void CacheService::ensureCacheDbParentDirExists() const
{
    QFileInfo fileInfo(_path);
    const auto &dir = fileInfo.dir();
    if (!dir.exists())
        dir.mkpath(".");
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
    const auto &applicationCache = _applicationsORM.retrieveById(application.getId());
    if (applicationCache.getId() == application.getId()) {
        _applicationsORM.update(application);

        // replace bundles
        _applicationBundleORM.removeApplicationBundles(application.getId());
        for (const auto &bundle : application.getBundles())
            _applicationBundleORM.save(bundle);
    } else {
        _applicationsORM.create(application);
        for (const auto &bundle : application.getBundles())
            _applicationBundleORM.save(bundle);
    }
}
ApplicationsList CacheService::listApplications()
{
    auto applications = _applicationsORM.retrieve();
    for (auto &app : applications) {
        const auto &bundles = _applicationBundleORM.retrieveApplicationBundles(app.getId());
        app.setBundles(bundles, 0);
    }

    return applications;
}
void CacheService::removeApplication(const Application &application)
{
    _applicationsORM.removeById(application.getId());
}
