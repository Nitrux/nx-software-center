//
// Created by alexis on 22/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_TESTREGISTRY_H
#define NOMAD_SOFTWARE_CENTER_TESTREGISTRY_H

#include <gtest/gtest.h>
#include <QSignalSpy>

#include "entities/Registry.h"
#include "interactors/TaskMetadata.h"
#include "entities/RecordMetadata.h"

namespace NX_SOFTWARE_CENTER_TESTS {
    class TestRegistry : public testing::Test {

    public:
        static void addSuccesfullInstallRecord(Registry *r, const char *test_app_id) {

            QVariantMap task = TestRegistry::createInstallTaskMetadata(test_app_id);
            task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_COMPLETED);

            r->handleTaskCompleted("", task);
        }

        static QVariantMap createInstallTaskMetadata(const char *app_id) {
            QVariantMap task;
            task.insert(TaskMetadata::KEY_TYPE, TaskMetadata::VALUE_TYPE_INSTALL);
            task.insert(TaskMetadata::KEY_APP_ID, app_id);

            return task;
        }

        static QVariantMap createUninstallTaskMetadata(const char *app_id) {
            QVariantMap task;
            task.insert(TaskMetadata::KEY_TYPE, TaskMetadata::VALUE_TYPE_REMOVE);
            task.insert(TaskMetadata::KEY_APP_ID, app_id);

            return task;
        }
    };

    TEST_F(TestRegistry, handleSuccessfullInstall) {
        Registry r;
        r.clearRecords();
        r.clearInstalledApplications();

        QSignalSpy spyRecordsChanged(&r, &Registry::recordsChanged);
        QSignalSpy spyInstalledApplicationsChanged(&r, &Registry::installedApplicationsChanged);

        const char *test_app_id = "testapp_v1.0";
        addSuccesfullInstallRecord(&r, test_app_id);

        GTEST_ASSERT_EQ(1, spyRecordsChanged.count());
        GTEST_ASSERT_EQ(1, spyInstalledApplicationsChanged.count());
        ASSERT_TRUE(r.getInstalledApplications().contains(test_app_id));
        GTEST_ASSERT_LT(0, r.getRecords().count());
    }

    TEST_F(TestRegistry, handleFailedInstall) {
        Registry r;
        r.clearRecords();
        r.clearInstalledApplications();

        QSignalSpy spyRecordsChanged(&r, &Registry::recordsChanged);
        QSignalSpy spyInstalledApplicationsChanged(&r, &Registry::installedApplicationsChanged);

        const char *test_app_id = "testapp_v1.0";

        QVariantMap task = TestRegistry::createInstallTaskMetadata(test_app_id);
        task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_FAILED);

        r.handleTaskCompleted("", task);

        GTEST_ASSERT_EQ(1, spyRecordsChanged.count());
        GTEST_ASSERT_EQ(0, spyInstalledApplicationsChanged.count());
        ASSERT_FALSE(r.getInstalledApplications().contains(test_app_id));
        GTEST_ASSERT_LT(0, r.getRecords().count());
    }

    TEST_F(TestRegistry, handleSuccessfullUninstall) {
        Registry r;
        r.clearRecords();
        r.clearInstalledApplications();

        const char *test_app_id = "testapp_v1.0";
        addSuccesfullInstallRecord(&r, test_app_id);

        QSignalSpy spyRecordsChanged(&r, &Registry::recordsChanged);
        QSignalSpy spyInstalledApplicationsChanged(&r, &Registry::installedApplicationsChanged);


        QVariantMap task = TestRegistry::createUninstallTaskMetadata(test_app_id);
        task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_COMPLETED);

        r.handleTaskCompleted("", task);

        GTEST_ASSERT_EQ(1, spyRecordsChanged.count());
        GTEST_ASSERT_EQ(1, spyInstalledApplicationsChanged.count());

        GTEST_ASSERT_EQ(0, r.getInstalledApplications().count());
        ASSERT_TRUE(!r.getInstalledApplications().contains(test_app_id));

        GTEST_ASSERT_LT(0, r.getRecords().count());
    }

    TEST_F(TestRegistry, handleFailedUninstall) {
        Registry r;
        r.clearRecords();
        r.clearInstalledApplications();

        const char *test_app_id = "testapp_v1.0";
        addSuccesfullInstallRecord(&r, test_app_id);

        QSignalSpy spyRecordsChanged(&r, &Registry::recordsChanged);
        QSignalSpy spyInstalledApplicationsChanged(&r, &Registry::installedApplicationsChanged);


        QVariantMap task = TestRegistry::createUninstallTaskMetadata(test_app_id);
        task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_FAILED);

        r.handleTaskCompleted("", task);

        GTEST_ASSERT_EQ(1, spyRecordsChanged.count());
        GTEST_ASSERT_EQ(0, spyInstalledApplicationsChanged.count());

        GTEST_ASSERT_EQ(1, r.getInstalledApplications().count());
        ASSERT_TRUE(r.getInstalledApplications().contains(test_app_id));

        GTEST_ASSERT_LT(0, r.getRecords().count());
    }

    TEST_F(TestRegistry, persistence) {
        { // ADD A RECORD TO THE REGISTRY
            Registry registry;
            const char *test_app_id = "testapp_v1.0";
            QVariantMap task = TestRegistry::createInstallTaskMetadata(test_app_id);
            task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_COMPLETED);

            registry.handleTaskCompleted("", task);
        }

        { // CHECK THAT THERE IS SOMETHING IN THERE
            Registry registry;
            ASSERT_LT(0, registry.getRecords().size());
            ASSERT_LT(0, registry.getInstalledApplications().size());

            // CLEAR
            registry.clearRecords();
            registry.clearInstalledApplications();
        }

        { // CHECK THAT IS EMPTY
            Registry registry;
            ASSERT_EQ(0, registry.getRecords().size());
            ASSERT_EQ(0, registry.getInstalledApplications().size());
        }
    }
}

#endif //NOMAD_SOFTWARE_CENTER_TESTREGISTRY_H
