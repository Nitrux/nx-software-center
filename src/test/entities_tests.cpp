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

    void testRegistry()
    {
        Registry registry;

        DummyChange change("coll_app_1");

        registry.registerChange(&change);
        QVERIFY(registry.runningChanges().size() == 0);
        change.execute();
        QVERIFY(registry.runningChanges().size() == 1);
        change.finish();
        QVERIFY(registry.runningChanges().size() == 0);
        QVERIFY(registry.installedReleaseIds().size() == 1);
        QVERIFY(registry.downloadedReleaseIds().size() == 1);
    }

    void testSystem()
    {
        System system;

        DummyRepository repository;
        Registry registry;

        QVERIFY(system.listAppIds().size() == 0);

        system.setRegistry(&registry);
        system.addRepository(&repository);

        QString appId = system.listAppIds().first();

        QVERIFY(system.appDetails(appId)["id"].toString() == appId);

        QString lastReleaseID = system.listAppReleaseIds(appId).first();

        QVERIFY(system.releaseDetails(appId, lastReleaseID)["id"] == appId) ;

        QString downloadChangeId = system.downloadRelease(appId, lastReleaseID);

        QVERIFY(registry.getChange(downloadChangeId) != nullptr);
        QVERIFY(registry.downloadedReleaseIds().contains(lastReleaseID));

        QString installChangeId = system.installRelease(appId, lastReleaseID);

        QVERIFY(registry.getChange(installChangeId) != nullptr);
        QVERIFY(registry.installedReleaseIds().contains(lastReleaseID));

        QString uninstallChangeId = system.uninstallRelease(appId, lastReleaseID);

        QVERIFY(registry.getChange(uninstallChangeId) != nullptr);
        QVERIFY(!registry.installedReleaseIds().contains(lastReleaseID));

        QString removeChangeId = system.removeRelease(appId, lastReleaseID);

        QVERIFY(registry.getChange(removeChangeId) != nullptr);
        QVERIFY(!registry.downloadedReleaseIds().contains(lastReleaseID));

    }
};
QTEST_MAIN(EntitiesTests)
#include "entities_tests.moc"
