#include "TestCacheService.h"

// libraries
#include <QTest>

// local
#include "services/Application.h"

void TestCacheService::initTestCase()
{
    _cacheFile = _cacheDir.path() + "/cache.db";
    qDebug() << _cacheFile;

    _service = new CacheService(_cacheFile);

    int version = 1;
    for (auto &bundle : _demoAppBundles)
        fillBundleData(version++, "DemoApp", bundle);

    version = 1;
    for (auto &bundle : _otherAppBundles)
        fillBundleData(version++, "OtherApp", bundle);
}

void TestCacheService::fillBundleData(int version, const QString &appName, ApplicationBundle &bundle) const
{
    bundle.path = "/tmp/file/path/" + appName + "_v" + QString::number(version) + ".AppImage";
    bundle.size = 1024 * version;
    bundle.lastModified = QDateTime::currentDateTime();
    bundle.hashSumMD5 = "x0x0x0";
    bundle.bundleType = ApplicationBundle::AppImage;
    bundle.data.setId("org.nxos.nxsc." + appName.toLower());
    bundle.data.setName("Demo App");
    bundle.data.setVersion(QString::number(version) + ".0.0");
}

void TestCacheService::cleanupTestCase()
{
    QFile::remove(_cacheFile);
    _cacheDir.remove();

    delete _service;
}

void TestCacheService::testInitDB()
{
    QVERIFY(_service->isOperational());
}
void TestCacheService::testSaveApplications()
{
    _service->saveApplication(Application(_demoAppBundles[0]));
}
void TestCacheService::testListApplication()
{
    const auto results = _service->listApplications();
    QCOMPARE(results.length(), 1);

    const auto &demoAppCopy = results.first();
    const auto &demoAppData = _demoAppBundles[0].data;
    QCOMPARE(demoAppCopy.getId(), demoAppData.getId());
    QCOMPARE(demoAppCopy.getData().toJson(), demoAppData.toJson());

    const auto &retrievedBundles = demoAppCopy.getBundles();
    QCOMPARE(retrievedBundles.size(), 1);

    const auto &demoAppBundleCopy = retrievedBundles.first();
    QCOMPARE(demoAppBundleCopy.path, _demoAppBundles[0].path);
    QCOMPARE(demoAppBundleCopy.size, _demoAppBundles[0].size);
    QCOMPARE(demoAppBundleCopy.lastModified, _demoAppBundles[0].lastModified);
    QCOMPARE(demoAppBundleCopy.hashSumMD5, _demoAppBundles[0].hashSumMD5);
    QCOMPARE(demoAppBundleCopy.bundleType, _demoAppBundles[0].bundleType);
    QCOMPARE(demoAppBundleCopy.data.toJson(), _demoAppBundles[0].data.toJson());
}

void TestCacheService::testModifyApplication()
{
    _service->saveApplication(Application(_demoAppBundles[1]));

    const auto &apps = _service->listApplications();
    QCOMPARE(apps.size(), 1);
    const auto &first = apps.first();
    QCOMPARE(first.getData().getVersion(), _demoAppBundles[1].data.getVersion());
}
void TestCacheService::testRemoveApplication()
{
    _service->removeApplication(Application(_demoAppBundles[0]));
    const auto results = _service->listApplications();
    QCOMPARE(results.length(), 0);
}

QTEST_GUILESS_MAIN(TestCacheService);