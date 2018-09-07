//
// Created by alexis on 22/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_TESTREGISTRY_H
#define NOMAD_SOFTWARE_CENTER_TESTREGISTRY_H

#include <gtest/gtest.h>
#include <QSignalSpy>

#include "entities/TaskLogger.h"
#include "tasks/TaskMetadata.h"

namespace NX_SOFTWARE_CENTER_TESTS {
    class TestTaskLogger : public testing::Test {

    public:
        static void addSuccesfullDeployRecord(TaskLogger *r, const char *test_app_id) {
            QVariantMap task = TestTaskLogger::createDeployTask(test_app_id);
            task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_COMPLETED);

            r->handleTaskCompleted(task);
        }

        static QVariantMap createDeployTask(const char *app_id) {
            QVariantMap task;
            task.insert(TaskMetadata::KEY_TYPE, TaskMetadata::VALUE_TYPE_DELPOY);
            task.insert(TaskMetadata::KEY_APP_ID, app_id);

            return task;
        }

        static QVariantMap createRemoveTask(const char *app_id) {
            QVariantMap task;
            task.insert(TaskMetadata::KEY_TYPE, TaskMetadata::VALUE_TYPE_REMOVE);
            task.insert(TaskMetadata::KEY_APP_ID, app_id);

            return task;
        }
    };

    TEST_F(TestTaskLogger, handleSuccessfullInstall) {
        TaskLogger r;
        r.clearRecords();
        QSignalSpy spyRecordsChanged(&r, &TaskLogger::recordsChanged);

        const char *test_app_id = "testapp_v1.0";
        addSuccesfullDeployRecord(&r, test_app_id);

        GTEST_ASSERT_EQ(1, spyRecordsChanged.count());
        GTEST_ASSERT_LT(0, r.getRecords().count());
    }

    TEST_F(TestTaskLogger, handleFailedInstall) {
        TaskLogger r;
        r.clearRecords();

        QSignalSpy spyRecordsChanged(&r, &TaskLogger::recordsChanged);
        const char *test_app_id = "testapp_v1.0";

        QVariantMap task = TestTaskLogger::createDeployTask(test_app_id);
        task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_FAILED);

        r.handleTaskCompleted(task);

        GTEST_ASSERT_EQ(1, spyRecordsChanged.count());
        GTEST_ASSERT_LT(0, r.getRecords().count());
    }

    TEST_F(TestTaskLogger, handleSuccessfullUninstall) {
        TaskLogger r;
        r.clearRecords();

        const char *test_app_id = "testapp_v1.0";
        addSuccesfullDeployRecord(&r, test_app_id);

        QSignalSpy spyRecordsChanged(&r, &TaskLogger::recordsChanged);

        QVariantMap task = TestTaskLogger::createRemoveTask(test_app_id);
        task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_COMPLETED);

        r.handleTaskCompleted(task);

        GTEST_ASSERT_EQ(1, spyRecordsChanged.count());

        GTEST_ASSERT_LT(0, r.getRecords().count());
    }

    TEST_F(TestTaskLogger, handleFailedUninstall) {
        TaskLogger r;
        r.clearRecords();

        const char *test_app_id = "testapp_v1.0";
        addSuccesfullDeployRecord(&r, test_app_id);

        QSignalSpy spyRecordsChanged(&r, &TaskLogger::recordsChanged);


        QVariantMap task = TestTaskLogger::createRemoveTask(test_app_id);
        task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_FAILED);

        r.handleTaskCompleted(task);

        GTEST_ASSERT_EQ(1, spyRecordsChanged.count());
        GTEST_ASSERT_LT(0, r.getRecords().count());
    }

    TEST_F(TestTaskLogger, persistence) {
        { // ADD A RECORD TO THE REGISTRY
            TaskLogger registry;
            const char *test_app_id = "testapp_v1.0";
            QVariantMap task = TestTaskLogger::createDeployTask(test_app_id);
            task.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_COMPLETED);

            registry.handleTaskCompleted(task);
        }

        { // CHECK THAT THERE IS SOMETHING IN THERE
            TaskLogger registry;
            ASSERT_GT(registry.getRecords().size(), 0);

            // CLEAR
            registry.clearRecords();
        }

        { // CHECK THAT IS EMPTY
            TaskLogger registry;
            ASSERT_EQ(0, registry.getRecords().size());
        }
    }
}

#endif //NOMAD_SOFTWARE_CENTER_TESTREGISTRY_H
