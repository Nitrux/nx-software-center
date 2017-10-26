#include <QTest>
#include <QSignalSpy>
#include <QDebug>

#include "../entities/app.h"
#include "../entities/change.h"
#include "../entities/release.h"
#include "../entities/repository.h"
#include "../entities/registry.h"
#include "../entities/system.h"

#include "../interactors/searchapplicationsinteractor.h"
#include "../interactors/searchapplicationsinteractorlistener.h"

#include "dummychange.h"
#include "dummyinstallchange.h"
#include "dummyrelease.h"
#include "dummyrepository.h"

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

class InteractorsTests : public QObject
{
    Q_OBJECT
private slots:

    void testsearchapplicationsinteractorMainScenario() {

        DummySearchApplicationsInteractorListener listener;
        DummyRepository repository;
        repository.updateCache();

        QString searchString("app_1");
        SearchApplicationsInteractor interactor(searchString,
                                                QList<Repository *>({&repository}),
                                                &listener);
        interactor.execute();

        QVERIFY(listener.appFound);
        for (QVariant app: listener.m_applicationsList)
        {
            QVariantHash appData = app.toHash();
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
};
QTEST_MAIN(InteractorsTests)
#include "interactors_tests.moc"
