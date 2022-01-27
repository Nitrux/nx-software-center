#include "TestApplicationsRegistry.h"

#include <QSharedPointer>
#include <QSignalSpy>
#include <QTestAccessibility>

#include "ApplicationData.h"
#include "registry/ApplicationsRegistry.h"

void TestApplicationsRegistry::initTestCase()
{
    registry = new ApplicationsRegistry({}, {});
    qRegisterMetaType<ApplicationData>("ApplicationData");

    b1 = new ApplicationBundle("/tmp/fake_bundle_1.AppImage");
    b1->app->setVersion("1");
    b1->app->setName("B1 Name");
    b1->app->setId("com.nxos.fake-app");

    b2 = new ApplicationBundle("/tmp/fake_bundle_2.AppImage");
    b2->app->setId("com.nxos.fake-app");
    b2->app->setVersion("2");
    b2->app->setName("B2 Name");
}

void TestApplicationsRegistry::cleanupTestCase()
{
    delete registry;
}

void TestApplicationsRegistry::addBundle()
{
    QSignalSpy spy(registry, SIGNAL(applicationAdded(ApplicationData)));

    registry->addBundle(*b1);

    auto retrievedApp = registry->getApplication(b1->app->getId());
    QCOMPARE(b1->app->getId(), retrievedApp.getId());
    QCOMPARE(spy.count(), 1);
    QVERIFY(registry->applicationExist(b1->app->getId()));
}

void TestApplicationsRegistry::updateApplicationOnUpgrade()
{
    QSignalSpy spy(registry, SIGNAL(applicationUpdated(ApplicationData)));
    registry->addBundle(*b2);

    QCOMPARE(spy.count(), 1);

    // inspect signal arguments
    auto arguments = spy.takeFirst();
    const auto &applicationDataVariant = arguments.at(0);
    auto updatedApp = qvariant_cast<ApplicationData>(applicationDataVariant);
    QCOMPARE(updatedApp.getName(), b2->app->getName());
    QVERIFY(registry->applicationExist(b1->app->getId()));
}

void TestApplicationsRegistry::updateApplicationOnDowngrade()
{
    QSignalSpy spy(registry, SIGNAL(applicationUpdated(ApplicationData)));
    registry->removeBundleByPath(b2->path);

    QCOMPARE(spy.count(), 1);

    // inspect signal arguments
    auto arguments = spy.takeFirst();
    const auto &applicationDataVariant = arguments.at(0);
    auto updatedApp = qvariant_cast<ApplicationData>(applicationDataVariant);
    QCOMPARE(updatedApp.getName(), b1->app->getName());
    QVERIFY(registry->applicationExist(b1->app->getId()));
}

void TestApplicationsRegistry::removeApplication()
{
    QSignalSpy spy(registry, SIGNAL(applicationRemoved(ApplicationData)));
    registry->removeBundle(*b1);

    QCOMPARE(spy.count(), 1);

    // inspect signal arguments
    auto arguments = spy.takeFirst();
    const auto &applicationDataVariant = arguments.at(0);
    auto updatedApp = qvariant_cast<ApplicationData>(applicationDataVariant);
    QCOMPARE(updatedApp.getName(), b1->app->getName());

    QCOMPARE(registry->applicationExist(b1->app->getId()), false);
}

QTEST_GUILESS_MAIN(TestApplicationsRegistry)