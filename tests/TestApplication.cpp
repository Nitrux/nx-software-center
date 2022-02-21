#include "TestApplication.h"

#include "services/Application.h"

#include "TestUtils.hpp"

#define APP_ID_MOCK "org.nxos.no-app"

void TestApplication::createFromBundle()
{
    ApplicationBundle bundle("/non/existent/path");
    bundle.data.setId(APP_ID_MOCK);

    Application app(bundle);
    QCOMPARE(app.getId(), APP_ID_MOCK);
}

void TestApplication::testAddBundle()
{
    auto app = Application();
    const auto &appData = app.getData();

    app.addBundle(mockApplicationBundle(APP_ID_MOCK, "0.2.0"));
    QCOMPARE(app.getId(), APP_ID_MOCK);
    QCOMPARE(appData.getVersion(), "0.2.0");

    app.addBundle(mockApplicationBundle(APP_ID_MOCK, "0.3.0"));
    QCOMPARE(appData.getVersion(), "0.3.0");

    // ensure newer bundle is keep as main
    app.addBundle(mockApplicationBundle(APP_ID_MOCK, "0.1.0"));
    QCOMPARE(appData.getVersion(), "0.3.0");

    // both bundles share different paths, the old registry must be kept
    QCOMPARE(app.getBundles().length(), 3);
}

void TestApplication::testReplaceBundle()
{
    ApplicationBundle b1("/non/existent/path");
    b1.data.setId(APP_ID_MOCK);
    b1.data.setVersion("0.1.0");

    auto app = Application();
    app.addBundle(b1);

    QCOMPARE(app.getId(), b1.data.getId());
    QCOMPARE(app.getId(), APP_ID_MOCK);

    ApplicationBundle b2("/non/existent/path");
    b2.data.setId(APP_ID_MOCK);
    b2.data.setVersion("0.2.0");
    app.addBundle(b2);

    const auto &appData = app.getData();
    QCOMPARE(appData.getVersion(), "0.2.0");

    // both bundles share the same path, therefore the first must be replaced
    QCOMPARE(app.getBundles().length(), 1);
}

QTEST_GUILESS_MAIN(TestApplication);
