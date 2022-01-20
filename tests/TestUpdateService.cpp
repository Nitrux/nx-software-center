#include "TestUpdateService.h"

#include <QSignalSpy>

#define APPLICATIONS_DIR_MOCK "/tmp/applications_dir_mock"

void TestUpdateService::initTestCase()
{
    _tempdir.setPath(APPLICATIONS_DIR_MOCK);
    _tempdir.mkpath(APPLICATIONS_DIR_MOCK);

    _outdatedAppImagePath = QString(APPLICATIONS_DIR_MOCK) + "/appimagetool-old.AppImage";

    QFile appimage(SAMPLE_APPIMAGE_PATH);
    appimage.copy(_outdatedAppImagePath);
}

void TestUpdateService::cleanupTestCase()
{
    _tempdir.removeRecursively();
}

void TestUpdateService::testCheckUpdatesOnAppWithoutBundles()
{
    QSignalSpy signalSpy(&service, &UpdateService::isBusyChanged);

    service.checkUpdates({ApplicationData()});

    signalSpy.wait();
    QCOMPARE(service.isBusy(), false);
    QCOMPARE(signalSpy.count(), 2);
    QCOMPARE(service.getUpdatesAvailableCounter(), 0);
}

void TestUpdateService::testCheckUpdates()
{
    const ApplicationBundle bundle(_outdatedAppImagePath);
    const ApplicationData application(bundle);
    ApplicationsList applicationsList = {application};

    QSignalSpy signalSpy(&service, &UpdateService::isBusyChanged);
    service.checkUpdates(applicationsList);

    signalSpy.wait();
    QCOMPARE(signalSpy.count(), 2);
    QCOMPARE(service.getUpdatesAvailableCounter(), 1);
}

void TestUpdateService::testCheckUpdate()
{
    const ApplicationBundle bundle(_outdatedAppImagePath);
    const ApplicationData application(bundle);

    QSignalSpy signalSpy(&service, &UpdateService::isBusyChanged);
    service.checkUpdates({application});

    signalSpy.wait();
    QCOMPARE(signalSpy.count(), 2);
    QCOMPARE(service.getUpdatesAvailableCounter(), 1);
}

QTEST_MAIN(TestUpdateService);