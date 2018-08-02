//
// Created by alexis on 16/01/18.
//

#include <QTemporaryFile>
#include <QFileInfo>
#include <QSignalSpy>

#include <gtest/gtest.h>
#include "gateways/FileDownload.h"

namespace NX_SOFTWARE_CENTER_TESTS {

    class TestDownload : public testing::Test {
    protected:
        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
    };


    TEST_F(TestDownload, downloadFile) {
        QTemporaryFile temporaryFile;
        QFileInfo source(TEST_DATA_DIR "echo-x86_64-8.25.AppImage");

        GTEST_ASSERT_EQ(true, source.exists());
        if (temporaryFile.open() )  {
            FileDownload f("file://" TEST_DATA_DIR "echo-x86_64-8.25.AppImage", temporaryFile.fileName());
            f.setProgressNotificationsEnabled(true);

            QSignalSpy signalSpyCompleted(&f, &FileDownload::completed);
            QSignalSpy signalSpyStopped(&f, &FileDownload::stopped);
            QSignalSpy signalSpyProgress(&f, &FileDownload::progress);

            f.start();
            signalSpyCompleted.wait();

            GTEST_ASSERT_LT(0, signalSpyProgress.count());
            GTEST_ASSERT_EQ(1, signalSpyCompleted.count());
            GTEST_ASSERT_EQ(0, signalSpyStopped.count());

            GTEST_ASSERT_EQ(true, temporaryFile.exists());
            GTEST_ASSERT_EQ(source.size(), temporaryFile.size());
        } else
            GTEST_FAIL();
    }
}
