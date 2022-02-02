#pragma once
// system

// libraries
#include <QtSql/QSqlDatabase>

// local
#include "../ApplicationBundle.h"
#include "ApplicationBundleORM.h"
#include "ApplicationORM.h"

class CacheService : public QObject
{
    Q_OBJECT

public:
    explicit CacheService(QString path);
    ~CacheService() override;

    bool isOperational() const;

    ApplicationsList listApplications();
    Q_SLOT void saveApplication(const Application &application);
    Q_SLOT void deleteApplication(const Application &application);

private:
    QString _path;
    QSqlDatabase _database;
    ApplicationORM _applicationsORM;
    ApplicationBundleORM _bundlesORM;

    void ensureCacheDbParentDirExists() const;
};