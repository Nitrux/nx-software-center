#include "TestApplicationData.h"
#include "ApplicationData.h"

#define APP_ID_MOCK "org.nxos.no-app"

void TestApplicationData::createFromBundle()
{
    ApplicationBundle bundle("/non/existent/path");
    bundle.app->setId(APP_ID_MOCK);

    ApplicationData app(bundle);
    QCOMPARE(app.getId(), APP_ID_MOCK);
}

void TestApplicationData::testAddBundle()
{
    ApplicationBundle b1("/non/existent/path");
    b1.app->setId(APP_ID_MOCK);
    b1.app->setVersion("0.1.0");

    auto app = ApplicationData(QVariantMap());
    app.addBundle(b1);

    QCOMPARE(app.getId(), b1.app->getId());
    QCOMPARE(app.getId(), APP_ID_MOCK);

    ApplicationBundle b2("/non/existent/path");
    b2.app->setId(APP_ID_MOCK);
    b2.app->setVersion("0.2.0");
    app.addBundle(b2);

    QCOMPARE(app.getVersion(), "0.2.0");
}

QTEST_GUILESS_MAIN(TestApplicationData);
