//
// Created by alexis on 23/01/18.
//

#include <QSignalSpy>

#include <gtest/gtest.h>

#include "entities/Updater.h"
#include "DummySource.h"

namespace NX_SOFTWARE_CENTER_TESTS {
    class TestUpdater : public testing::Test {
    };

    TEST_F(TestUpdater, update) {
        DummySource source;
        Repository repository;
        Updater u(&repository, {&source});

        ASSERT_FALSE(u.getIsReady());
        ASSERT_FALSE(u.getIsWorking());

        QSignalSpy spyIsReadyChanged(&u, &Updater::isReadyChanged);
        QSignalSpy spyIsWorkingChanged(&u, &Updater::isWorkingChanged);
        QSignalSpy spyRepositoryChanged(&repository, &Repository::changed);

        u.update();

        ASSERT_LE(1, spyIsReadyChanged.count());
        ASSERT_LE(1, spyIsWorkingChanged.count());
        ASSERT_EQ(repository.getAll(), source.getOutputApplications());
        ASSERT_EQ(1, spyRepositoryChanged.count());
    }
}
