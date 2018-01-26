#include <gtest/gtest.h>

#include <QList>
#include <QSignalSpy>

#include "entities/Application.h"
#include "gateways/AppImageHubSource.h"
#include "FakeDownloadManager.h"


namespace NX_SOFTWARE_CENTER_TESTS {
    class TestAppImageHubSource : public testing::Test {
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
    };

    TEST_F(TestAppImageHubSource, fetchApplications) {
        QNetworkAccessManager networkAccessManager;
        FakeDownloadManager downloadManager(&networkAccessManager);
        AppImageHubSource s(&downloadManager);

        static QList<Application> req_results;
        static QString req_error;
        QObject::connect(&s, &AppImageHubSource::fetchedAllApplications,
                         [=](QList<Application> results) { req_results = results; });

        QObject::connect(&s, &AppImageHubSource::fetchError,
                         [=](QString error) { req_error = error; });

        QSignalSpy spyRes(&s, &AppImageHubSource::fetchedAllApplications);
        QSignalSpy spyErr(&s, &AppImageHubSource::fetchError);

        s.fetchAllApplications();

        spyRes.wait(60000);

        ASSERT_EQ(1, spyRes.count());
        ASSERT_EQ(0, spyErr.count());
        ASSERT_FALSE(req_results.isEmpty());
        for (const Application &a: req_results) {
            qDebug() << a.getId() << a.getDownloadUrl();
        }
    }
}
