#include <QTest>
#include <QFile>
#include <QDebug>
#include <QSignalSpy>

#include "../entities/app.h"
#include "../entities/change.h"
#include "../entities/release.h"
#include "../entities/repository.h"
#include "../entities/registry.h"
#include "../entities/system.h"

#include "../interactors/searchapplicationsinteractor.h"
#include "../interactors/searchapplicationsinteractorlistener.h"
#include "../interactors/downloadappimagereleaseinteractor.h"
#include "../interactors/downloadappimagereleaseinteractorlistener.h"

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

class InteractorsTests : public QObject
{
    Q_OBJECT
private slots:

    void testsearchapplicationsinteractorMainScenario() {
        DummySearchApplicationsInteractorListener listener;
        DummyRepository repository;
        repository.updateCache();

        QString searchString(repository.appName);
        SearchApplicationsInteractor interactor(searchString,
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

        DummySearchApplicationsInteractorListener listener;
        DummyRepository repository;
        repository.updateCache();

        SearchApplicationsInteractor interactor(QString(),
                                                QList<Repository *>({&repository}),
                                                &listener);
        interactor.execute();

        QVERIFY(listener.appFound);
    }

    void testsearchapplicationsinteractorNoApplicationsMatchingScenario() {

        DummySearchApplicationsInteractorListener listener;
        DummyRepository repository;
        repository.updateCache();

        QString searchString("not_existent_app");
        SearchApplicationsInteractor interactor(searchString,
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
        QVERIFY( registryFilePath.compare(listener.m_filePath) == 0);

        downloadManager.removeTmpFile();

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

};
QTEST_MAIN(InteractorsTests)
#include "interactors_tests.moc"
