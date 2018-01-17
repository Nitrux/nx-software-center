//
// Created by alexis on 16/01/18.
//

#include <QTemporaryFile>
#include <QFileInfo>
#include <QSignalSpy>

#include <gtest/gtest.h>
#include "gateways/FileDownload.h"
#include "gateways/ContentDownload.h"

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

            QSignalSpy signalSpyCompleted(&f, &Download::completed);
            QSignalSpy signalSpyStopped(&f, &Download::stopped);
            QSignalSpy signalSpyProgress(&f, &Download::progress);

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

    TEST_F(TestDownload, downloadContent) {
        QFile source(TEST_DATA_DIR "example_releases_page.html");

        GTEST_ASSERT_EQ(true, source.exists());
        if (source.open(QIODevice::ReadOnly) )  {
            ContentDownload f("file://" TEST_DATA_DIR "example_releases_page.html");
            f.setProgressNotificationsEnabled(true);

            QSignalSpy signalSpyCompleted(&f, &Download::completed);
            QSignalSpy signalSpyStopped(&f, &Download::stopped);
            QSignalSpy signalSpyProgress(&f, &Download::progress);

            f.start();
            signalSpyCompleted.wait();

            GTEST_ASSERT_LT(0, signalSpyProgress.count());
            GTEST_ASSERT_EQ(1, signalSpyCompleted.count());
            GTEST_ASSERT_EQ(0, signalSpyStopped.count());

            GTEST_ASSERT_EQ(source.readAll(), f.getContent());
        } else
            GTEST_FAIL();
    }
}
