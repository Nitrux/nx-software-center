#pragma once
// system

// libraries
#include <QSqlDatabase>

// local
#include "../Application.h"

class ApplicationBundleORM;
class ApplicationDataORM;

class ApplicationORM
{
public:
    explicit ApplicationORM(const QSqlDatabase &database);
    virtual ~ApplicationORM();

    void init();

    bool applicationTableExists() const;
    void createApplicationTable();

    void createOrUpdateApplication(const Application &application);

    ApplicationsList listApplications();

private:
    QSqlDatabase _database;
//    ApplicationDataORM *_dataORM;
//    ApplicationBundleORM *_bundleORM;
    void createApplication(const Application &application) const;
};
