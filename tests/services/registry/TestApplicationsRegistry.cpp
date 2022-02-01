#include "TestApplicationsRegistry.h"

#include <QSharedPointer>
#include <QSignalSpy>
#include <QTestAccessibility>

#include "services/Application.h"
#include "services/registry/ApplicationsRegistry.h"

void TestApplicationsRegistry::initTestCase()
{
    registry = new ApplicationsRegistry({}, {});
    qRegisterMetaType<Application>("Application");

    b1 = new ApplicationBundle("/tmp/fake_bundle_1.AppImage");
    b1->data.setVersion("1");
    b1->data.setName("B1 Name");
    b1->data.setId("com.nxos.fake-app");

    b2 = new ApplicationBundle("/tmp/fake_bundle_2.AppImage");
    b2->data.setId("com.nxos.fake-app");
    b2->data.setVersion("2");
    b2->data.setName("B2 Name");
}

void TestApplicationsRegistry::cleanupTestCase()
{
    delete registry;
}

void TestApplicationsRegistry::addBundle()
{
    QSignalSpy spy(registry, SIGNAL(applicationAdded(Application)));

    registry->addBundle(*b1);

    auto retrievedApp = registry->getApplication(b1->data.getId());
    QCOMPARE(b1->data.getId(), retrievedApp.getId());
    QCOMPARE(spy.count(), 1);
    QVERIFY(registry->applicationExist(b1->data.getId()));
}

void TestApplicationsRegistry::updateApplicationOnUpgrade()
{
    QSignalSpy spy(registry, SIGNAL(applicationUpdated(Application)));
    registry->addBundle(*b2);

    QCOMPARE(spy.count(), 1);

    // inspect signal arguments
    auto arguments = spy.takeFirst();
    const auto &applicationDataVariant = arguments.at(0);
    auto updatedApp = qvariant_cast<Application>(applicationDataVariant);
    const auto &updatedAppData = updatedApp.getData();
    QCOMPARE(updatedAppData.getName(), b2->data.getName());
    QVERIFY(registry->applicationExist(b1->data.getId()));
}

void TestApplicationsRegistry::updateApplicationOnDowngrade()
{
    QSignalSpy spy(registry, SIGNAL(applicationUpdated(Application)));
    registry->removeBundleByPath(b2->path);

    QCOMPARE(spy.count(), 1);

    // inspect signal arguments
    auto arguments = spy.takeFirst();
    const auto &applicationDataVariant = arguments.at(0);
    auto updatedApp = qvariant_cast<Application>(applicationDataVariant);
    const auto &updatedAppData = updatedApp.getData();
    QCOMPARE(updatedAppData.getName(), b1->data.getName());
    QVERIFY(registry->applicationExist(b1->data.getId()));
}

void TestApplicationsRegistry::removeApplication()
{
    QSignalSpy spy(registry, SIGNAL(applicationRemoved(Application)));
    registry->removeBundle(*b1);

    QCOMPARE(spy.count(), 1);

    // inspect signal arguments
    auto arguments = spy.takeFirst();
    const auto &applicationDataVariant = arguments.at(0);
    auto updatedApp = qvariant_cast<Application>(applicationDataVariant);
    const auto &updatedAppData = updatedApp.getData();
    QCOMPARE(updatedAppData.getName(), b1->data.getName());

    QCOMPARE(registry->applicationExist(b1->data.getId()), false);
}

QTEST_GUILESS_MAIN(TestApplicationsRegistry)