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
    QSignalSpy signalSpy(&service, &UpdateService::checkTaskRunningChanged);

    service.checkUpdates({ApplicationData()});

    signalSpy.wait();
    QCOMPARE(service.isCheckTaskRunning(), false);
    QCOMPARE(signalSpy.count(), 2);
    QCOMPARE(service.getUpdatesAvailableCounter(), 0);
}

void TestUpdateService::testCheckUpdates()
{
    const ApplicationBundle bundle(_outdatedAppImagePath);
    const ApplicationData application(bundle);
    ApplicationsList applicationsList = {application};

    QSignalSpy signalSpy(&service, &UpdateService::checkTaskRunningChanged);
    service.checkUpdates(applicationsList);

    signalSpy.wait();
    QCOMPARE(signalSpy.count(), 2);
    QCOMPARE(service.getUpdatesAvailableCounter(), 1);
}

void TestUpdateService::testCheckUpdate()
{
    const ApplicationBundle bundle(_outdatedAppImagePath);
    const ApplicationData application(bundle);

    QSignalSpy signalSpy(&service, &UpdateService::checkTaskRunningChanged);
    QSignalSpy spyUpdateDataChanged(&service, &UpdateService::applicationUpdateDataChanged);
    service.checkUpdates({application});

    signalSpy.wait();
    QCOMPARE(signalSpy.count(), 2);
    QCOMPARE(service.getUpdatesAvailableCounter(), 1);

    QCOMPARE(spyUpdateDataChanged.count(), 1);
    QList<QVariant> arguments = spyUpdateDataChanged.takeFirst();
    auto updateData = qvariant_cast<ApplicationUpdateData>(arguments.at(0));
    QVERIFY(updateData.updateAvailable);
}

void TestUpdateService::testUpdate()
{
    const ApplicationBundle bundle(_outdatedAppImagePath);
    const ApplicationData application(bundle);

    QSignalSpy spyStatus(&service, &UpdateService::updateTaskRunningChanged);
    QSignalSpy spyUpdateDataChanged(&service, &UpdateService::applicationUpdateDataChanged);
    service.update(application);

    spyStatus.wait(60000);
    QCOMPARE(spyStatus.count(), 2);
    QCOMPARE(service.getUpdatesAvailableCounter(), 0);

    QCOMPARE(spyUpdateDataChanged.count(), 1);
    QList<QVariant> arguments = spyUpdateDataChanged.takeFirst();
    auto updateData = qvariant_cast<ApplicationUpdateData>(arguments.at(0));
    QVERIFY(updateData.updateAvailable == false);
}

QTEST_MAIN(TestUpdateService);