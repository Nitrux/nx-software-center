#include <QDebug>
#include <QSignalSpy>
#include <QTest>

#include "../entities/app.h"
#include "../entities/change.h"
#include "../entities/progresslistener.h"
#include "../entities/registry.h"
#include "../entities/release.h"
#include "../entities/repository.h"

#include "dummychange.h"
#include "dummyinstallchange.h"
#include "dummyrelease.h"
#include "dummyrepository.h"

class EntitiesTests : public QObject {
  Q_OBJECT
private slots:

  void testAppReleasesOrdering() {
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

  void testRepository() {
    DummyRepository *repository = new DummyRepository();
    QVERIFY(repository->list().size() == 0);
    repository->updateCache();
    QVERIFY(repository->list().size() == 5);
  }

  void testChange() {
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
  void testRegistry() {
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
};
QTEST_MAIN(EntitiesTests)
#include "entities_tests.moc"
