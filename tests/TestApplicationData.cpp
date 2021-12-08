#include "TestApplicationData.h"
#include "ApplicationData.h"

#define APP_ID_MOCK "org.nxos.no-app"

void TestApplicationData::createFromBundle()
{
    auto bundle = new ApplicationBundle("/non/existent/path", new ApplicationData());
    bundle->app->setId(APP_ID_MOCK);

    auto app = ApplicationData(bundle);
    QCOMPARE(app.getId(), APP_ID_MOCK);
}

void TestApplicationData::testAddBundle()
{
    auto b1 = new ApplicationBundle("/non/existent/path", new ApplicationData());
    b1->app->setId(APP_ID_MOCK);
    b1->app->setVersion("0.1.0");

    auto app = ApplicationData();
    app.addBundle(b1);

    QCOMPARE(app.getId(), b1->app->getId());
    QCOMPARE(app.getId(), APP_ID_MOCK);

    auto b2 = new ApplicationBundle("/non/existent/path", new ApplicationData());
    b2->app->setId(APP_ID_MOCK);
    b2->app->setVersion("0.2.0");
    app.addBundle(b2);

    QCOMPARE(app.getVersion(), "0.2.0");
}

QTEST_MAIN(TestApplicationData);
