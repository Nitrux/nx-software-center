#pragma once
// system

// libraries
#include <QSqlDatabase>

// local
#include "../Application.h"
#include "ApplicationBundleORM.h"

class ApplicationORM
{
public:
    explicit ApplicationORM(const QSqlDatabase &database);
    virtual ~ApplicationORM();

    void init();

    [[nodiscard]] bool applicationTableExists() const;
    void createApplicationTable();

    Application retrieveById(const QString &id);
    void create(const Application &application) const;
    void update(const Application &application);
    void removeById(const QString &id);

    ApplicationsList retrieve();

private:
    QSqlDatabase _database;
};
