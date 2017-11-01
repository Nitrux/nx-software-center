#include <QTest>
#include <QFile>
#include <QDebug>
#include <QSignalSpy>
#include <QTemporaryFile>

#include "../entities/app.h"
#include "../entities/change.h"
#include "../entities/release.h"
#include "../entities/repository.h"
#include "../entities/registry.h"

#include "../interactors/searchapplicationsinteractor.h"
#include "../interactors/searchapplicationsinteractorlistener.h"
#include "../interactors/downloadappimagereleaseinteractor.h"
#include "../interactors/downloadappimagereleaseinteractorlistener.h"
#include "../interactors/removeappimagereleaseinteractor.h"
#include "../interactors/removeappimagereleaseinteractorlistener.h"

#include "dummychange.h"
#include "dummyinstallchange.h"
#include "dummyrelease.h"
#include "dummyrepository.h"
#include "dummydownloadmanager.h"

class DummySearchApplicationsInteractorListener : public SearchApplicationsInteractorListener
{
public:
    DummySearchApplicationsInteractorListener() {}

    virtual void handleApplicationsList(const QVariantList &applicationsList) {
        m_applicationsList = applicationsList;
        appFound = true;
    }
    virtual void handleNoApplicationsFound() {
        noAppFound = true;
    }

    void clean() {
        appFound = false;
        noAppFound = false;
    }

    bool appFound = false;
    bool noAppFound = false;
    QVariantList m_applicationsList;
};

class DummyDownloadAppImageReleaseInteractorListener : public DownloadAppImageReleaseInteractorListener
{
public:
    virtual ~DummyDownloadAppImageReleaseInteractorListener() {}

    virtual void progress(const int progress, const int total, const QString statusMessage) {
        qDebug() << "Download progress " << progress << " of " << total << statusMessage;
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

class DummyRemoveAppImageReleaseInteractorListener : public RemoveAppImageReleaseInteractorListener
{
public:
    virtual ~DummyRemoveAppImageReleaseInteractorListener() {}

    virtual void error(const QString &errorMessage) {
        qDebug() << errorMessage;
        m_errorMessage = errorMessage;
        m_removeFailed = true;
    }

    virtual void finished() {
        m_removeCompleted = true;
    }

    bool m_removeCompleted = false;
    bool m_removeFailed = false;

    QString m_errorMessage;
};

class InteractorsTests : public QObject
{
    Q_OBJECT
private slots:

    void testsearchapplicationsinteractorMainScenario() {\
        Registry registry;
        DummySearchApplicationsInteractorListener listener;
        DummyRepository repository;
        repository.updateCache();

        QString searchString(repository.appName);
        SearchApplicationsInteractor interactor(searchString,
                                                &registry,
                                                QList<Repository *>({&repository}),
                                                &listener);
        interactor.execute();

        QVERIFY(listener.appFound);
        for (QVariant app: listener.m_applicationsList)
        {
            QVariantMap appData = app.toMap();
            auto appId = appData["id"].toString();
            auto appName = appData["name"].toString();
            auto appDescription = appData["description"].toString();

            QVERIFY(!appId.isEmpty());
            QVERIFY(appName.contains(searchString) ||
                    appDescription.contains(searchString));

//            qDebug() << appId << " " << appName << " " << appDescription;
        }
    }

    void testsearchapplicationsinteractorEmptySearchScenario() {
        Registry registry;
        DummySearchApplicationsInteractorListener listener;
        DummyRepository repository;
        repository.updateCache();

        SearchApplicationsInteractor interactor(QString(),
                                                &registry,
                                                QList<Repository *>({&repository}),
                                                &listener);
        interactor.execute();

        QVERIFY(listener.appFound);
    }

    void testsearchapplicationsinteractorNoApplicationsMatchingScenario() {
        Registry registry;
        DummySearchApplicationsInteractorListener listener;
        DummyRepository repository;
        repository.updateCache();

        QString searchString("not_existent_app");
        SearchApplicationsInteractor interactor(searchString,
                                                &registry,
                                                QList<Repository *>({&repository}),
                                                &listener);
        interactor.execute();

        QVERIFY(listener.noAppFound);
        QVERIFY(listener.m_applicationsList.size() == 0);
    }

    void testDownloadAppImageReleaseInteractorMainScenario()
    {
        DummyDownloadAppImageReleaseInteractorListener listener;

        DummyDownloadManager downloadManager;
        DummyRepository repository;
        repository.updateCache();

        Registry registry;

        DownloadAppImageReleaseInteractor interactor("app_1", "r2", QList<Repository *>({&repository}),
                                                     &downloadManager, &registry, &listener);

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

    void testDownloadAppImageReleaseInteractorDownloadErrorScenario()
    {
        DummyDownloadAppImageReleaseInteractorListener listener;

        DummyDownloadManager downloadManager(true);
        DummyRepository repository;
        repository.updateCache();

        Registry registry;

        DownloadAppImageReleaseInteractor interactor("app_1", "r2", QList<Repository *>({&repository}),
                                                     &downloadManager, &registry, &listener);

        interactor.execute();
        QVERIFY(listener.m_downloadFailed);
        QVERIFY(!listener.m_errorMessage.isEmpty());

        QFile file(listener.m_filePath);
        QVERIFY(!file.exists());
    }

    void testDownloadAppImageReleaseInteractorInvalidApplicationReleaseIdScenario()
    {
        DummyDownloadAppImageReleaseInteractorListener listener;

        DummyDownloadManager downloadManager;
        DummyRepository repository;
        repository.updateCache();

        Registry registry;

        DownloadAppImageReleaseInteractor interactor("app_invalid", "r2_invalid", QList<Repository *>({&repository}),
                                                     &downloadManager, &registry, &listener);

        interactor.execute();
        QVERIFY(listener.m_downloadFailed);
        QVERIFY(!listener.m_errorMessage.isEmpty());

        QFile file(listener.m_filePath);
        QVERIFY(!file.exists());
    }

    void testRemoveAppImageReleaseInteractorMainScenario() {
        DummyRemoveAppImageReleaseInteractorListener listener;

        QString appId = "testApp";
        QString releaseId = "testRelease";

        QTemporaryFile tmpFile;
        tmpFile.open();
        tmpFile.write("HELLO CRUEL WORLD!");
        tmpFile.close();

        Q_ASSERT(tmpFile.exists());

        Registry registry;
        registry.registerReleaseDownload(appId, releaseId, tmpFile.fileName());

        RemoveAppImageReleaseInteractor interactor(appId, releaseId, &registry, &listener);
        interactor.execute();

        QVERIFY(!tmpFile.exists());
        QVERIFY(listener.m_removeCompleted);
        QVERIFY(!listener.m_removeFailed);
    }

    void testRemoveAppImageReleaseInteractorNotDownloadedScenario() {
        DummyRemoveAppImageReleaseInteractorListener listener;

        QString appId = "--";
        QString releaseId = "--";

        Registry registry;

        RemoveAppImageReleaseInteractor interactor(appId, releaseId, &registry, &listener);
        interactor.execute();

        QVERIFY(listener.m_removeCompleted);
        QVERIFY(!listener.m_removeFailed);
        QVERIFY(listener.m_errorMessage.isEmpty());
    }
};
QTEST_MAIN(InteractorsTests)
#include "interactors_tests.moc"
