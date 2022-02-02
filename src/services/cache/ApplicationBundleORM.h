#pragma once
// system

// libraries
#include <QSqlDatabase>

// local
#include "../ApplicationData.h"

class ApplicationBundleORM
{
public:
    explicit ApplicationBundleORM(const QSqlDatabase &database);
    void createOrUpdate(const ApplicationBundle &bundle);

    bool isReady() const;

private:
    bool _ready;
    QSqlDatabase _database;
    const QString _tableName;
    bool tableExists() const;
    void createTable();
};