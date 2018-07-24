//
// Created by alexis on 7/20/18.
//

#ifndef NX_SOFTWARE_CENTER_TESTLOCALAPPLICATIONREGISTRY_H
#define NX_SOFTWARE_CENTER_TESTLOCALAPPLICATIONREGISTRY_H

#include <gtest/gtest.h>
#include <QTemporaryDir>
#include <gateways/DeployedApplicationsRegistry.h>

namespace NX_SOFTWARE_CENTER_TESTS {
    class DeployedApplicationsRegistryWrapper : public DeployedApplicationsRegistry {
    public:
        using DeployedApplicationsRegistry::getSha1;
        using DeployedApplicationsRegistry::listApplicationFiles;
        using DeployedApplicationsRegistry::extractFileInfo;
        using DeployedApplicationsRegistry::synchronizeCache;
        using DeployedApplicationsRegistry::appImageInfoCache;
    };

    class TestDeployedApplicationsRegistry : public testing::Test {
    protected:
        DeployedApplicationsRegistryWrapper repository;
        QTemporaryDir cacheDir;

        void SetUp() override {
            repository.setApplicationsDir(TEST_DATA_DIR);
            repository.setCacheDir(cacheDir.path());
        }

        void TearDown() override {
        }
    };


    TEST_F(TestDeployedApplicationsRegistry, listApplicationFiles) {
        QStringList result = repository.listApplicationFiles();
        QStringList expected = {TEST_DATA_DIR "echo-x86_64-8.25.AppImage"};
        ASSERT_EQ(result, expected);
    }

    TEST_F(TestDeployedApplicationsRegistry, extractFileInfo) {
        repository.extractFileInfo(TEST_DATA_DIR "echo-x86_64-8.25.AppImage");
        auto sha1 = repository.getSha1(TEST_DATA_DIR "echo-x86_64-8.25.AppImage");

        ASSERT_TRUE(QFile::exists(cacheDir.path() + "/" + sha1 + "/AppImageInfo.json"));
        ASSERT_TRUE(QFile::exists(cacheDir.path() + "/" + sha1 + "/AppImageIcon"));
    }

    TEST_F(TestDeployedApplicationsRegistry, synchronizeCache) {
        QDir dirToRemove(cacheDir.path());
        ASSERT_TRUE(dirToRemove.mkdir("should_be_removed"));

        repository.synchronizeCache();

        const auto appInfo = repository.appImageInfoCache.first();
        ASSERT_EQ(appInfo.id, QString("echo.desktop"));
        ASSERT_FALSE(dirToRemove.exists("should_be_removed"));
    }

    TEST_F(TestDeployedApplicationsRegistry, listApplications) {
        auto results = repository.listApplications();

        const auto abstract = results.first();
        ASSERT_EQ(abstract.id, QString("echo.desktop"));
        ASSERT_EQ(abstract.filePath.toStdString(), std::string(TEST_DATA_DIR "echo-x86_64-8.25.AppImage"));
    }

    TEST_F(TestDeployedApplicationsRegistry, getApplicationInfo) {
        auto result = repository.getApplicationInfo(TEST_DATA_DIR "echo-x86_64-8.25.AppImage");
        ASSERT_EQ(result.id.toStdString(), std::string("echo.desktop"));
    }

    TEST_F(TestDeployedApplicationsRegistry, registerFile) {
        AppImageInfo info;
        info.id = "no_echo.Desktop";
        repository.registerFile(TEST_DATA_DIR "echo-x86_64-8.25.AppImage", info);
        auto result = repository.getApplicationInfo(TEST_DATA_DIR "echo-x86_64-8.25.AppImage");
        ASSERT_EQ(result.id.toStdString(), std::string("no_echo.Desktop"));
    }
}

#endif //NX_SOFTWARE_CENTER_TESTLOCALAPPLICATIONREGISTRY_H
