#include "TestApplicationsRegistry.h"

#include <QSharedPointer>
#include <QTestAccessibility>

#include "ApplicationData.h"
#include "ApplicationsRegistry.h"

void TestApplicationsRegistry::initTestCase()
{
}

void TestApplicationsRegistry::cleanupTestCase()
{
}

void TestApplicationsRegistry::addBundle()
{
    auto bundle = QSharedPointer<ApplicationBundle>(new ApplicationBundle("/tmp/fake_bundle.AppImage", new ApplicationData()));
    bundle->app->setId("com.nxos.fake-app");

    auto registry = ApplicationsRegistry({});
    registry.addBundle(bundle);

    auto retrievedApp = registry.getApplication(bundle->app->getId());
    qDebug() << retrievedApp.getId();
}

QTEST_MAIN(TestApplicationsRegistry)