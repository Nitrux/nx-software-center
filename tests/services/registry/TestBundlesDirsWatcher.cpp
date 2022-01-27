#include "TestBundlesDirsWatcher.h"
#include "ApplicationBundle.h"
#include "registry/BundlesDirsWatcher.h"

#include <QRandomGenerator>
#include <QSignalSpy>

#define APPLICATIONS_DIR_MOCK "/tmp/watched_dir"

void TestBundlesDirsWatcher::initTestCase()
{
    dirToWatch.mkpath(APPLICATIONS_DIR_MOCK);
    dirToWatch.setPath(APPLICATIONS_DIR_MOCK);

    QString existentAppimagePath = APPLICATIONS_DIR_MOCK "/existent.AppImage";
    copyAppImage(existentAppimagePath);
    QFileInfo existentAppImageInfo(existentAppimagePath);
    QMap<QString, QDateTime> fileCache = {{existentAppimagePath, existentAppImageInfo.lastModified()}};

    watcher = new BundlesDirsWatcher({dirToWatch.path()}, fileCache);
    qRegisterMetaType<ApplicationBundle>("ApplicationBundle");
    watcher->moveToThread(&_workerThread);
    _workerThread.start();
}
void TestBundlesDirsWatcher::cleanupTestCase()
{
    dirToWatch.removeRecursively();
    _workerThread.exit();
    _workerThread.wait(10);
}
void TestBundlesDirsWatcher::copyAppImage(const QString &path)
{
    QFile appimage(SAMPLE_APPIMAGE_PATH);
    appimage.copy(path);
}

void TestBundlesDirsWatcher::testWatchBundleCacheInitialization()
{
    QSignalSpy spyAppImageAdded(watcher, SIGNAL(bundleAdded(ApplicationBundle)));
    watcher->checkAllDirs();
    QTest::qWait(100);

    QCOMPARE(spyAppImageAdded.count(), 0);
}

void TestBundlesDirsWatcher::testWatchBundleAdded()
{
    QSignalSpy spyAppImageAdded(watcher, SIGNAL(bundleAdded(ApplicationBundle)));

    QString path = APPLICATIONS_DIR_MOCK "/f1.AppImage";
    copyAppImage(path);

    QTest::qWait(100);

    QCOMPARE(spyAppImageAdded.count(), 1);
}

void TestBundlesDirsWatcher::testWatchBundleChanged()
{
    QSignalSpy spyAppImageUpdate(watcher, SIGNAL(bundleUpdated(ApplicationBundle)));

    QString path = APPLICATIONS_DIR_MOCK "/f1.AppImage";
    copyAppImage(path);

    QTest::qWait(100);

    QCOMPARE(spyAppImageUpdate.count(), 0);
}
void TestBundlesDirsWatcher::testWatchBundleRemoved()
{
    QSignalSpy spyAppImageRemoved(watcher, SIGNAL(bundleRemoved(QString)));

    QString path = APPLICATIONS_DIR_MOCK "/f1.AppImage";
    copyAppImage(path);

    QFile::remove(path);
    QTest::qWait(100);

    QCOMPARE(spyAppImageRemoved.count(), 1);
}

QTEST_MAIN(TestBundlesDirsWatcher);
