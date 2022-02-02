#include "TestCacheService.h"

// libraries
#include <QTest>

// local
#include "services/Application.h"
#include "services/ApplicationBundle.h"
#include "services/cache/CacheService.h"

void TestCacheService::initTestCase()
{
    _cacheFile = _cacheDir.path() + "/cache.db";
}

void TestCacheService::cleanupTestCase()
{
    _cacheDir.remove();
}

void TestCacheService::testInitDB()
{
    CacheService helper(_cacheFile);
    QVERIFY(helper.isOperational());
}
void TestCacheService::testSaveApplication()
{
    const Application application;
    CacheService service(_cacheFile);
    service.saveApplication(application);
}

QTEST_GUILESS_MAIN(TestCacheService);