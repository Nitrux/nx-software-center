#pragma once
// system

// libraries
#include <QSqlDatabase>

// local
#include "../Application.h"

class ApplicationORM
{
public:
    explicit ApplicationORM(const QSqlDatabase &database);

private:
    QSqlDatabase _database;
};
