#pragma once
// system

// libraries
#include <QSqlDatabase>

// local

class ApplicationDataORM
{
public:
    explicit ApplicationDataORM(const QSqlDatabase &database);

private:
    QSqlDatabase _database;
};
