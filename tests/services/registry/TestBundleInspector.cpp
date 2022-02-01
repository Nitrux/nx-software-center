#include "TestBundleInspector.h"
#include "Application.h"
#include "registry/BundleInspector.h"

#include <QStandardPaths>

void TestBundleInspector::initTestCase()
{
}
void TestBundleInspector::cleanupTestCase()
{
}
void TestBundleInspector::inspectBundle()
{
    auto inspector = BundleInspector(SAMPLE_APPIMAGE_PATH);
    auto bundle = inspector.getData();

    QCOMPARE(bundle.path, QString(SAMPLE_APPIMAGE_PATH));
    QCOMPARE(bundle.data.getId(), QString("appimagetool"));
    QCOMPARE(bundle.data.getName(), QString("appimagetool"));
    QCOMPARE(bundle.data.getDescription(), QString("Tool to generate AppImages from AppDirs"));
    QCOMPARE(bundle.data.getXdgCategories(), QStringList({"Development"}));
    QVERIFY(!bundle.hashSumMD5.isEmpty());

    QString expectedIconPath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first() + "/icons/appimagetool";
    QCOMPARE(bundle.data.getIcon(), expectedIconPath);
    QVERIFY(QFile::exists(expectedIconPath));
}

QTEST_MAIN(TestBundleInspector)
