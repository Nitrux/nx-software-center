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
void TestCacheService::testSaveApplication()
{
    const Application application;
    _service->saveApplication(application);
}

QTEST_GUILESS_MAIN(TestCacheService);