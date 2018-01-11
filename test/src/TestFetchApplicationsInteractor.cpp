#include <gtest/gtest.h>

#include <QSignalSpy>

#include "interactors/FetchApplicationsInteractor.h"
#include "DummySource.h"
#include "BrokenDummySource.h"


namespace NX_SOFTWARE_CENTER_TESTS {
    class TestFetchApplicationsInteractor : public testing::Test {
    protected:
        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
    };

    TEST_F(TestFetchApplicationsInteractor, fetchApplications) {
        DummySource s;
        FetchApplicationsInteractor *i = new FetchApplicationsInteractor({&s});

        static QList<Application> request_results;
        static QStringList request_errors;
        QObject::connect(i, &FetchApplicationsInteractor::completed,
                         [=]() {
                             request_errors = i->getErrors();
                             request_results = i->getResults();
                         });

        QSignalSpy spy(i, &FetchApplicationsInteractor::completed);

        i->execute();

        ASSERT_EQ(1, spy.count());
        ASSERT_EQ(s.getOutputApplications(), request_results);
        ASSERT_TRUE(request_errors.isEmpty());

        delete i;
    }

    TEST_F(TestFetchApplicationsInteractor, fetchApplicationsBroken) {
        BrokenDummySource s;
        FetchApplicationsInteractor *i = new FetchApplicationsInteractor({&s});

        static QList<Application> request_results;
        static QStringList request_errors;
        QObject::connect(i, &FetchApplicationsInteractor::completed,
                         [=]() {
            request_errors = i->getErrors();
            request_results = i->getResults();
                         });

        QSignalSpy spy(i, &FetchApplicationsInteractor::completed);

        i->execute();

        ASSERT_EQ(1, spy.count());
        ASSERT_TRUE(request_results.isEmpty());
        ASSERT_FALSE(request_errors.isEmpty());

        delete i;
    }
}
