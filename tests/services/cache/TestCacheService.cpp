#include "TestCacheService.h"

// libraries
#include <QTest>

// local
#include "services/Application.h"

void TestCacheService::initTestCase()
{
    //    _cacheFile = _cacheDir.path() + "/cache.db";
    _cacheFile = "/tmp/test-nxsc/cache.db";
    _service = new CacheService(_cacheFile);

    _demoAppData.setId("org.nxos.nxsc.demoapp");
    _demoAppData.setName("Demo App");
    _demoAppData.setVersion("1.0.0");
}

void TestCacheService::cleanupTestCase()
{
    _cacheDir.remove();
    delete _service;
}

void TestCacheService::testInitDB()
{
    QVERIFY(_service->isOperational());
}
void TestCacheService::testSaveApplicationWithoutBundles()
{
    _service->saveApplication(Application(_demoAppData));
}
void TestCacheService::testListOneApplication()
{
    const auto result = _service->listApplications();
    QCOMPARE(result.length(), 1);

    const auto &first = result.first();
    QCOMPARE(first.getId(), _demoAppData.getId());
    QCOMPARE(first.getData().toJson(), _demoAppData.toJson());
}

QTEST_GUILESS_MAIN(TestCacheService);