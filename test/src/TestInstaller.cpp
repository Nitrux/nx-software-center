//
// Created by alexis on 7/4/18.
//

#include <gtest/gtest.h>
#include <QSignalSpy>

#include "entities/Installer.h"
#include <entities/InstallTask.h>
#include "interactors/TaskMetadata.h"
#include "gateways/RestClient.h"

namespace NX_SOFTWARE_CENTER_TESTS {
class TestInstaller : public testing::Test {
};

TEST_F(TestInstaller, installApplication)
{
    auto restClient = new RestClient("http://apps.nxos.org/api");
    Installer i;
    i.setRestClient(restClient);
    auto task = i.buildInstallLatestReleaseTask("test.desktop");
    QSignalSpy spyCompleted(task, &InstallTask::completed);
    QSignalSpy spyFailed(task, &InstallTask::failed);
    task->start();
    spyCompleted.wait();
    ASSERT_TRUE(spyCompleted.count()>0);
    ASSERT_TRUE(spyFailed.count() == 0);
    restClient->deleteLater();
}
}