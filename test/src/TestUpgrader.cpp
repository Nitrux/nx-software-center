//
// Created by alexis on 25/01/18.
//

#include <gtest/gtest.h>

#include <entities/Upgrader.h>
#include <entities/Repository.h>
#include <QtTest/QSignalSpy>

namespace NX_SOFTWARE_CENTER_TESTS {
    class TestUpgrader : public testing::Test {
    };

    TEST_F(TestUpgrader, checkForUpgrades) {
        Repository repository;

        Application application1("test", "1");
        Application application2("test", "2");
        repository.add(application1);
        repository.add(application2);

        Upgrader upgrader;
        upgrader.setRepository(&repository);

        upgrader.handleInstalledApplicationsChanged({"test-1"});
        UpgradeList upgrades = upgrader.getUpgradableApplications();

        ASSERT_TRUE(upgrades.contains(Upgrade("test-1","test-2")));

        Application application3("test", "3");
        QSignalSpy spy(&upgrader, &Upgrader::upgradableApplicationsChanged);

        repository.add(application3);

        ASSERT_LT(0, spy.count());
    }
}
