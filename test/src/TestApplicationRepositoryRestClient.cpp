#ifndef NOMAD_SOFTWARE_CENTER_TESTAPPLICATIONREPOSITORYRESTCLIENT_H
#define NOMAD_SOFTWARE_CENTER_TESTAPPLICATIONREPOSITORYRESTCLIENT_H

#include <gtest/gtest.h>
#include <QSignalSpy>
#include <gateways/ApplicationsSearchRequest.h>

#include "gateways/ApplicationRepositoryRestClient.h"
#include "gateways/ApplicationGetRequest.h"
#include "gateways/ApplicationsSearchRequest.h"

namespace NX_SOFTWARE_CENTER_TESTS {
    class TestApplicationRepositoryRestClient : public testing::Test, public ApplicationRepositoryRestClient {
    public:
        TestApplicationRepositoryRestClient()
                : ApplicationRepositoryRestClient("http://apps.nxos.org/api") {}
    };

    TEST_F(TestApplicationRepositoryRestClient, buildEmptyQuery) {
        ApplicationsSearchRequest *request = dynamic_cast<ApplicationsSearchRequest *>(buildSearchRequest("", ""));
        auto result = request->getUrl();
        QUrl expected("http://apps.nxos.org/api/applications/search");
        ASSERT_EQ(expected.toEncoded(), result.toEncoded());
        request->deleteLater();
    }

    TEST_F(TestApplicationRepositoryRestClient, buildTextOnlyQuery) {
        ApplicationsSearchRequest *request = dynamic_cast<ApplicationsSearchRequest *>(buildSearchRequest("appimage",
                                                                                                          ""));
        auto result = request->getUrl();
        QUrl expected("http://apps.nxos.org/api/applications/search?query=appimage");
        qInfo() << expected;
        qInfo() << result;
        ASSERT_EQ(result.toString().toStdString(), expected.toString().toStdString());
        request->deleteLater();
    }

    TEST_F(TestApplicationRepositoryRestClient, buildCategoryOnlyQuery) {
        ApplicationsSearchRequest *request = dynamic_cast<ApplicationsSearchRequest *>(buildSearchRequest("",
                                                                                                          "Development"));
        auto result = request->getUrl();
        QUrl expected("http://apps.nxos.org/api/applications/search?category=Development");
        ASSERT_EQ(expected.toEncoded(), result.toEncoded());
        request->deleteLater();
    }

    TEST_F(TestApplicationRepositoryRestClient, buildTextAndCategoryQuery) {
        ApplicationsSearchRequest *request = dynamic_cast<ApplicationsSearchRequest *>(buildSearchRequest("appimage",
                                                                                                          "Development"));
        auto result = request->getUrl();
        QUrl expected("http://apps.nxos.org/api/applications/search?query=appimage&category=Development");
        ASSERT_EQ(expected.toEncoded(), result.toEncoded());
        request->deleteLater();
    }

    TEST_F(TestApplicationRepositoryRestClient, buildLimitOffsetQuery) {
        ApplicationsSearchRequest *request = dynamic_cast<ApplicationsSearchRequest *>(buildSearchRequest());
        request->setLimit(2);
        request->setOffset(10);
        auto result = request->getUrl();
        QUrl expected("http://apps.nxos.org/api/applications/search?offset=10&limit=2");
        ASSERT_EQ(expected.toEncoded(), result.toEncoded());
        request->deleteLater();
    }

    TEST_F(TestApplicationRepositoryRestClient, buildGetApplicationUrl) {
        auto request = buildGetApplicationRequest("appimaged.desktop");
        auto result = request->getUrl();
        QUrl expected(
                R"(http://apps.nxos.org/api/applications/appimaged.desktop?filter={"include":[{"releases":{"files":{}}}]})");
        ASSERT_EQ(expected.toEncoded(), result.toEncoded());
        request->deleteLater();
    }


    TEST_F(TestApplicationRepositoryRestClient, getUnexistentApplication) {
        auto request = buildGetApplicationRequest("_MISSING_APPLICATION_");
        QSignalSpy spy(request, &ApplicationGetRequest::failed);
        request->start();

        spy.wait();
        ASSERT_EQ(1, spy.count());
        request->deleteLater();
    }

    TEST_F(TestApplicationRepositoryRestClient, getExistentApplication) {
        auto request = buildGetApplicationRequest("appimaged.desktop");
        QSignalSpy spy(request, &ApplicationGetRequest::completed);
        request->start();

        spy.wait();
        ASSERT_EQ(1, spy.count());
        request->deleteLater();
    }

    TEST_F(TestApplicationRepositoryRestClient, searchApplications) {
        ApplicationsSearchRequest *request = dynamic_cast<ApplicationsSearchRequest *>(buildSearchRequest());
        QSignalSpy spy(request, &ApplicationsSearchRequest::completed);
        request->start();

        spy.wait();
        ASSERT_EQ(1, spy.count());
        request->deleteLater();
    }

}

#endif // NOMAD_SOFTWARE_CENTER_TESTAPPLICATIONREPOSITORYRESTCLIENT_H