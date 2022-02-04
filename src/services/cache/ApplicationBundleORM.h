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

    void init();
    [[nodiscard]] bool applicationBundleTableExists() const;

    QList<ApplicationBundle> retrieveByApplicationId(const QString &applicationId);

    void save(const ApplicationBundle &bundle);

    QList<ApplicationBundle> retrieveApplicationBundles(QString applicationId);

    void removeApplicationBundles(const QString &applicationId);

private:
    bool _ready;
    QSqlDatabase _database;

    void createTable();
};