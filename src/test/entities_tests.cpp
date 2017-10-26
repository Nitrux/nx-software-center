#include <QTest>
#include <QSignalSpy>
#include <QDebug>

#include "../entities/app.h"
#include "../entities/change.h"
#include "../entities/release.h"
#include "../entities/repository.h"
#include "../entities/registry.h"
#include "../entities/system.h"

#include "dummychange.h"
#include "dummyinstallchange.h"
#include "dummyrelease.h"
#include "dummyrepository.h"

class EntitiesTests : public QObject
{
    Q_OBJECT
private slots:

    void testAppReleasesOrdering()
    {
        App app("dummy app");
        DummyRelease release1("v 1", app.id);
        DummyRelease release2("v 2", app.id);
        DummyRelease release3("v 3", app.id);


        app.addRelease(&release3);
        app.addRelease(&release1);
        app.addRelease(&release2);

        auto last_release = app.lastRelease();
        QVERIFY(last_release == &release3);
    }

    void testRepository()
    {
        Repository * repository = new DummyRepository();
        QVERIFY(repository->list().size() == 0);
        repository->updateCache();
        QVERIFY(repository->list().size() == 5);
    }

    void testChange()
    {
        DummyInstallChange change("app", "v0.9");
        int currentProgress, totalProgress;
        QString statusMessage;

        change.progress(currentProgress, totalProgress, statusMessage);
        QVERIFY(currentProgress == 0);
        QVERIFY(totalProgress == 100);
        QVERIFY(!statusMessage.isEmpty());

        change.execute();

        change.progress(currentProgress, totalProgress, statusMessage);
        QVERIFY(currentProgress == 50);
        QVERIFY(totalProgress == 100);
        QVERIFY(!statusMessage.isEmpty());

        change.finish();

        change.progress(currentProgress, totalProgress, statusMessage);
        QVERIFY(currentProgress == 100);
        QVERIFY(totalProgress == 100);
        QVERIFY(!statusMessage.isEmpty());
    }
    void testRegistry()
    {
        Registry registry;

        DummyInstallChange change("app", "v1");
        registry.registerChange(&change);

        QVERIFY(registry.runningChanges().size() == 0);
        change.execute();
        QVERIFY(registry.runningChanges().size() == 1);
        change.finish();

        QVERIFY(registry.runningChanges().size() == 0);
        QVERIFY(!registry.getReleaseFilePath("app", "v1").isEmpty());
        QVERIFY(!registry.getReleaseInstalleFilePaths("app", "v1").isEmpty());
    }

    void testSystem()
    {
        System system;

        DummyRepository *repository = new DummyRepository();
        Registry *registry = new Registry();

        QVERIFY(system.listAppIds().size() == 0);

        repository->updateCache();

        system.setRegistry(registry);
        system.addRepository(repository);

        QStringList appIds = system.listAppIds();
        QVERIFY(appIds.size() > 0);
        if (appIds.size() > 0)
        {
            QString appId = system.listAppIds().first();
            QVERIFY(system.appDetails(appId)["id"].toString() == appId);


            QStringList releaseIDs = system.listAppReleaseIds(appId);
            QVERIFY(releaseIDs.size() > 0);
            if (releaseIDs.size() > 0)
            {
                QString lastReleaseID = releaseIDs.first();
                QVERIFY(system.releaseDetails(appId, lastReleaseID)["app_id"] == appId) ;


                auto verifyChange = [=](QString changeId)
                {
                    Change * change = registry->getChange(changeId);
                    DummyChange * dummyChange = dynamic_cast<DummyChange*>(change);
                    QVERIFY(change != nullptr);
                    QVERIFY(dummyChange != nullptr);

                    if (dummyChange)
                        dummyChange->finish();
                };

                QString downloadChangeId = system.downloadRelease(appId, lastReleaseID);
                verifyChange(downloadChangeId);
                QVERIFY(!registry->getReleaseFilePath(appId, lastReleaseID).isEmpty());

                QString installChangeId = system.installRelease(appId, lastReleaseID);
                verifyChange(installChangeId);
                QVERIFY(!registry->getReleaseInstalleFilePaths(appId, lastReleaseID).isEmpty());

                QString uninstallChangeId = system.uninstallRelease(appId, lastReleaseID);
                verifyChange(uninstallChangeId);
                QVERIFY(registry->getReleaseInstalleFilePaths(appId, lastReleaseID).isEmpty());

                QString removeChangeId = system.removeRelease(appId, lastReleaseID);
                verifyChange(removeChangeId);
                QVERIFY(registry->getReleaseFilePath(appId, lastReleaseID).isEmpty());
            }


            delete registry;
            delete repository;
        }
    }
};
QTEST_MAIN(EntitiesTests)
#include "entities_tests.moc"
