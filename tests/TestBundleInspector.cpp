#include "TestBundleInspector.h"
#include "ApplicationData.h"
#include "BundleInspector.h"

#include <QStandardPaths>

void TestBundleInspector::initTestCase()
{
}
void TestBundleInspector::cleanupTestCase()
{
}
void TestBundleInspector::inspectBundle()
{
    auto bundle = BundleInspector::inspect(SAMPLE_APPIMAGE_PATH);

    QCOMPARE(bundle.path, QString(SAMPLE_APPIMAGE_PATH));
    QCOMPARE(bundle.app->getId(), QString("appimagetool"));
    QCOMPARE(bundle.app->getName(), QString("appimagetool"));
    QCOMPARE(bundle.app->getDescription(), QString("Tool to generate AppImages from AppDirs"));
    QCOMPARE(bundle.app->getXdgCategories(), QStringList({"Development"}));

    QString expectedIconPath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first() + "/icons/appimagetool";
    QCOMPARE(bundle.app->getIcon(), expectedIconPath);
    QVERIFY(QFile::exists(expectedIconPath));
}

QTEST_MAIN(TestBundleInspector)
