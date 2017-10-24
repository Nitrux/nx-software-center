#include <QTest>
#include <QSignalSpy>
#include <QDebug>

#include "../entities/app.h"
#include "../entities/change.h"
#include "../entities/release.h"
#include "../entities/repository.h"

#include "dummychange.h"
#include "dummyrelease.h"

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

    void testRelease()
    {
    }

    void testChange()
    {
    }

    void testRepository()
    {
    }

    void testRegistry()
    {
    }
};
QTEST_MAIN(EntitiesTests)
#include "entities_tests.moc"
