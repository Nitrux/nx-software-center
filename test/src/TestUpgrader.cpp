//
// Created by alexis on 25/01/18.
//

#include <gtest/gtest.h>

#include <entities/Upgrader.h>

#include <QtCore/QTemporaryDir>
#include <QtTest/QSignalSpy>

#include "DummyApplicationRepository.h"

namespace NX_SOFTWARE_CENTER_TESTS {
    class TestUpgrader : public testing::Test {
    };

    TEST_F(TestUpgrader, checkForUpgrades) {
        QTemporaryDir applicationsTemporaryDir;
        QTemporaryDir cacheTemporaryDir;

        QFile::copy(TEST_DATA_DIR "echo-x86_64-8.25.AppImage",
                    applicationsTemporaryDir.path() + "/echo-x86_64-8.25.AppImage");

        DummyApplicationRepository dummyApplicationRepository;
        DeployedApplicationsRegistry deployedApplicationsRegistry;
        deployedApplicationsRegistry.setApplicationsDir(applicationsTemporaryDir.path());
        deployedApplicationsRegistry.setCacheDir(cacheTemporaryDir.path());

        Upgrader upgrader;
        upgrader.setApplicationRepository(&dummyApplicationRepository);
        upgrader.setDeployedApplicationsRegistry(&deployedApplicationsRegistry);

        QSignalSpy spy(&upgrader, &Upgrader::upgradesLookUpCompleted);

        upgrader.lookUpForUpgrades();

        ASSERT_EQ(1, spy.count());
        auto upgrades = upgrader.getUpgradableApplications();
        ASSERT_EQ(1, upgrades.size());
        ASSERT_EQ(upgrades.first().id, "echo.desktop");
    }
}
