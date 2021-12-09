#include "TestApplicationsDirsWatcher.h"
#include "ApplicationsDirsWatcher.h"

#include <QRandomGenerator>
#include <QSignalSpy>

#define FAKE_APPLICATIONS_DIR "/tmp/watched_dir"

void TestApplicationsDirsWatcher::initTestCase()
{
    dirToWatch.mkpath(FAKE_APPLICATIONS_DIR);
    dirToWatch.setPath(FAKE_APPLICATIONS_DIR);

    watcher = new ApplicationsDirsWatcher({dirToWatch.path()});
}
void TestApplicationsDirsWatcher::cleanupTestCase()
{
    dirToWatch.removeRecursively();
}
void TestApplicationsDirsWatcher::createFakeAppImage(const QString &path)
{
    QFile f(path);
    if (f.open(QFile::WriteOnly)) {
        f.write("\x7F");
        f.write("ELF");
    }
}

void TestApplicationsDirsWatcher::testWatchAppImageAdded()
{
    QSignalSpy spyAppImageAdded(watcher, SIGNAL(appimageAdded(QString)));

    QString path = FAKE_APPLICATIONS_DIR "/f1.AppImage";
    createFakeAppImage(path);

    QTest::qWait(100);

    QCOMPARE(spyAppImageAdded.count(), 1);

    auto arguments = spyAppImageAdded.takeFirst();
    QCOMPARE(arguments.at(0).toString(), path);
}

void TestApplicationsDirsWatcher::testWatchAppImageChanged()
{
    QSignalSpy spyAppImageUpdate(watcher, SIGNAL(appimageUpdate(QString)));

    QString path = FAKE_APPLICATIONS_DIR "/f1.AppImage";
    createFakeAppImage(path);

    QTest::qWait(100);

    QCOMPARE(spyAppImageUpdate.count(), 0);
}
void TestApplicationsDirsWatcher::testWatchAppImageRemoved()
{
    QSignalSpy spyAppImageRemoved(watcher, SIGNAL(appimageRemoved(QString)));

    QString path = FAKE_APPLICATIONS_DIR "/f1.AppImage";
    createFakeAppImage(path);

    QFile::remove(path);
    QTest::qWait(100);

    QCOMPARE(spyAppImageRemoved.count(), 1);
}

QTEST_MAIN(TestApplicationsDirsWatcher);
