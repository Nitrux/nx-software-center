#pragma once
// system

// libraries
#include <QObject>
#include <QSqlDatabase>
#include <QTemporaryFile>

// local

class TestApplicationBundleORM : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testInit();

    void testSaveAndRetrieveBundle();

    void testDeleteBundles();

    void cleanupTestCase();

private:
    QTemporaryFile _dbFile;
    QSqlDatabase _db;
};
