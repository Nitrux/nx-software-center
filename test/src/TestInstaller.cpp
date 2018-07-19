//
// Created by alexis on 7/4/18.
//

#include <gtest/gtest.h>
#include <QSignalSpy>
#include <appimage/appimage.h>
#include "entities/Installer.h"
#include <entities/InstallTask.h>
#include "DummyApplicationRepository.h"

namespace NX_SOFTWARE_CENTER_TESTS {
    class TestInstaller : public testing::Test {
    };

    TEST_F(TestInstaller, installApplication) {
        DummyApplicationRepository repository;
        Installer i;
        i.setRepository(&repository);
        auto task = i.buildInstallLatestReleaseTask("echo.desktop");
        QSignalSpy spyCompleted(task, &InstallTask::completed);
        QSignalSpy spyFailed(task, &InstallTask::failed);
        task->start();
        spyCompleted.wait();
        ASSERT_TRUE(spyCompleted.count() > 0);
        ASSERT_TRUE(spyFailed.count() == 0);

        appimage_unregister_in_system(TEST_DATA_DIR "echo-x86_64-8.25.AppImage", false);
        appimage_unregister_in_system(TEST_DATA_DIR "echo-x86_64-8.25.AppImage", false);
    }
}