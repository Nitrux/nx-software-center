#include <QDebug>
#include <QFile>
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QTest>

#include "../entities/app.h"
#include "../entities/change.h"
#include "../entities/registry.h"
#include "../entities/release.h"
#include "../entities/repository.h"

#include "../interactors/downloadappimagereleaseinteractor.h"
#include "../interactors/downloadappimagereleaseinteractorlistener.h"
#include "../interactors/fetchappimagehubinteractor.h"
#include "../interactors/removeappimagereleaseinteractor.h"
#include "../interactors/removeappimagereleaseinteractorlistener.h"
#include "../interactors/searchapplicationsinteractor.h"
#include "../interactors/searchapplicationsinteractorlistener.h"

#include "dummychange.h"
#include "dummydownloadmanager.h"
#include "dummyinstallchange.h"
#include "dummyrelease.h"
#include "dummyrepository.h"

class DummySearchApplicationsInteractorListener
    : public SearchApplicationsInteractorListener {
public:
  DummySearchApplicationsInteractorListener() {}

  virtual void handleApplicationsList(const QVariantList &applicationsList) {
    m_applicationsList = applicationsList;
    appFound = true;
  }
  virtual void handleNoApplicationsFound() { noAppFound = true; }

  void clean() {
    appFound = false;
    noAppFound = false;
  }

  bool appFound = false;
  bool noAppFound = false;
  QVariantList m_applicationsList;
};

class DummyDownloadAppImageReleaseInteractorListener
    : public DownloadAppImageReleaseInteractorListener {
public:
  virtual ~DummyDownloadAppImageReleaseInteractorListener() {}

  virtual void progress(const int progress, const int total,
                        const QString statusMessage) {
    qDebug() << "Download progress " << progress << " of " << total
             << statusMessage;
    m_progress_reported = true;
  }

  virtual void downloadComplete(const QString filePaht) {
    m_filePath = filePaht;
    m_downloadComplete = true;
  }

  virtual void error(const QString &errorMessage) {
    qDebug() << errorMessage;
    m_errorMessage = errorMessage;
    m_downloadFailed = true;
  }

  bool m_downloadComplete = false;
  bool m_downloadFailed = false;
  bool m_progress_reported = false;
  QString m_filePath;
  QString m_errorMessage;
};

class InteractorsTests : public QObject {
  Q_OBJECT

private slots:

  void testsearchapplicationsinteractorMainScenario() {
    Registry registry;
    DummySearchApplicationsInteractorListener listener;
    DummyRepository repository;
    repository.updateCache();

    QString searchString(repository.appName);
    SearchApplicationsInteractor interactor(
        searchString, &registry, QList<Repository *>({&repository}), &listener);
    interactor.execute();

    QVERIFY(listener.appFound);
    for (QVariant app : listener.m_applicationsList) {
      QVariantMap appData = app.toMap();
      auto appId = appData["id"].toString();
      auto appName = appData["name"].toString();
      auto appDescription = appData["description"].toString();

      QVERIFY(!appId.isEmpty());
      QVERIFY(appName.contains(searchString) ||
              appDescription.contains(searchString));
    }
  }

  void testsearchapplicationsinteractorEmptySearchScenario() {
    Registry registry;
    DummySearchApplicationsInteractorListener listener;
    DummyRepository repository;
    repository.updateCache();

    SearchApplicationsInteractor interactor(
        QString(), &registry, QList<Repository *>({&repository}), &listener);
    interactor.execute();

    QVERIFY(listener.appFound);
  }

  void testsearchapplicationsinteractorNoApplicationsMatchingScenario() {
    Registry registry;
    DummySearchApplicationsInteractorListener listener;
    DummyRepository repository;
    repository.updateCache();

    QString searchString("not_existent_app");
    SearchApplicationsInteractor interactor(
        searchString, &registry, QList<Repository *>({&repository}), &listener);
    interactor.execute();

    QVERIFY(listener.noAppFound);
    QVERIFY(listener.m_applicationsList.size() == 0);
  }

  void testDownloadAppImageReleaseInteractorMainScenario() {
    DummyDownloadAppImageReleaseInteractorListener listener;

    DummyDownloadManager downloadManager;
    DummyRepository repository;
    repository.updateCache();

    Registry registry;

    DownloadAppImageReleaseInteractor interactor(
        "app_1", "r2", QList<Repository *>({&repository}), &downloadManager,
        &registry, &listener);

    interactor.execute();
    QVERIFY(listener.m_progress_reported);
    QVERIFY(listener.m_downloadComplete);
    QVERIFY(!listener.m_filePath.isEmpty());

    QString registryFilePath = registry.getReleaseFilePath("app_1", "r2");
    QVERIFY(registryFilePath.compare(listener.m_filePath) == 0);

    QFile file(registryFilePath);
    QVERIFY(file.exists());
    file.remove();

    downloadManager.removeTmpFile(listener.m_filePath);
  }

  void testDownloadAppImageReleaseInteractorDownloadErrorScenario() {
    DummyDownloadAppImageReleaseInteractorListener listener;

    DummyDownloadManager downloadManager(true);
    DummyRepository repository;
    repository.updateCache();

    Registry registry;

    DownloadAppImageReleaseInteractor interactor(
        "app_1", "r2", QList<Repository *>({&repository}), &downloadManager,
        &registry, &listener);

    interactor.execute();
    QVERIFY(listener.m_downloadFailed);
    QVERIFY(!listener.m_errorMessage.isEmpty());

    QFile file(listener.m_filePath);
    QVERIFY(!file.exists());
  }

  void
  testDownloadAppImageReleaseInteractorInvalidApplicationReleaseIdScenario() {
    DummyDownloadAppImageReleaseInteractorListener listener;

    DummyDownloadManager downloadManager;
    DummyRepository repository;
    repository.updateCache();

    Registry registry;

    DownloadAppImageReleaseInteractor interactor(
        "app_invalid", "r2_invalid", QList<Repository *>({&repository}),
        &downloadManager, &registry, &listener);

    interactor.execute();
    QVERIFY(listener.m_downloadFailed);
    QVERIFY(!listener.m_errorMessage.isEmpty());

    QFile file(listener.m_filePath);
    QVERIFY(!file.exists());
  }

  void testRemoveAppImageReleaseInteractorMainScenario() {
    QString appId = "testApp";
    QString releaseId = "testRelease";

    QTemporaryFile tmpFile;
    tmpFile.open();
    tmpFile.write("HELLO CRUEL WORLD!");
    tmpFile.close();

    Q_ASSERT(tmpFile.exists());

    Registry registry;
    registry.registerReleaseDownload(appId, releaseId, tmpFile.fileName());

    RemoveAppImageReleaseInteractor interactor(appId, releaseId, &registry);

    QSignalSpy spy(&interactor, &RemoveAppImageReleaseInteractor::finished);
    try {
      interactor.execute();
    } catch (QException) {
      QFAIL("Exception throw");
    }

    QCOMPARE(spy.count(), 1);
    QVERIFY(!tmpFile.exists());
  }

  void testRemoveAppImageReleaseInteractorNotDownloadedScenario() {
    QString appId = "--";
    QString releaseId = "--";

    Registry registry;

    RemoveAppImageReleaseInteractor interactor(appId, releaseId, &registry);
    QSignalSpy spy(&interactor, &RemoveAppImageReleaseInteractor::finished);
    bool failed = false;
    try {
      interactor.execute();
    } catch (QException) {
      failed = true;
    }

    QVERIFY(failed);
    QCOMPARE(spy.count(), 0);
  }
};

QTEST_MAIN(InteractorsTests)
#include "interactors_tests.moc"
