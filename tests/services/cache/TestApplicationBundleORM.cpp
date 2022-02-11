#include "TestApplicationBundleORM.h"

// libraries
#include <QTest>

// local
#include "../../TestUtils.hpp"
#include <services/ApplicationBundle.h>
#include <services/cache/ApplicationBundleORM.h>

void TestApplicationBundleORM::initTestCase()
{
    _dbFile.open();

    qDebug() << _dbFile.fileName();
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(_dbFile.fileName());
    _db.open();

    QVERIFY(_db.isOpen());
}
void TestApplicationBundleORM::cleanupTestCase()
{
    _db.close();
    _dbFile.close();
}
void TestApplicationBundleORM::testInit()
{
    ApplicationBundleORM orm(_db);
    orm.init();
    QVERIFY(orm.applicationBundleTableExists());
}
void TestApplicationBundleORM::testSaveAndRetrieveBundle()
{
    ApplicationBundle bundle = mockApplicationBundle("demoApp", "1");

    ApplicationBundleORM orm(_db);
    orm.init();

    orm.save(bundle);
    const auto results = orm.retrieveApplicationBundles(bundle.data.getId());
    QCOMPARE(results.size(), 1);

    const auto &retrievedBundle = results.first();
    QCOMPARE(retrievedBundle, bundle);
}
void TestApplicationBundleORM::testDeleteBundles()
{
    ApplicationBundleORM orm(_db);
    orm.init();

    const auto resultsBeforeDelete = orm.retrieveApplicationBundles("demoapp");
    QCOMPARE(resultsBeforeDelete.size(), 1);

    orm.removeApplicationBundles("demoapp");

    const auto resultsAfterDelete = orm.retrieveApplicationBundles("demoapp");
    QCOMPARE(resultsAfterDelete.size(), 0);
}

QTEST_GUILESS_MAIN(TestApplicationBundleORM);