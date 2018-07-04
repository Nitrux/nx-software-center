#ifndef NOMAD_SOFTWARE_CENTER_TESTRESTCLIENT_H
#define NOMAD_SOFTWARE_CENTER_TESTRESTCLIENT_H

#include <gtest/gtest.h>
#include <QSignalSpy>
#include <gateways/ApplicationsSearchRequest.h>

#include "gateways/RestClient.h"
#include "gateways/GetApplicationRequest.h"
#include "gateways/ApplicationsSearchRequest.h"

namespace NX_SOFTWARE_CENTER_TESTS {
class TestRestClient : public testing::Test, public RestClient {
public:
    TestRestClient()
            :RestClient("http://apps.nxos.org/api") { }
};

TEST_F(TestRestClient, buildEmptyQuery)
{
    auto request = buildSearchRequest("", "");
    auto result = request->getUrl();
    QUrl expected("http://apps.nxos.org/api/applications/search");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
    request->deleteLater();
}

TEST_F(TestRestClient, buildTextOnlyQuery)
{
    auto request = buildSearchRequest("appimage", "");
    auto result = request->getUrl();
    QUrl expected("http://apps.nxos.org/api/applications/search?query=appimage");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
    request->deleteLater();
}

TEST_F(TestRestClient, buildCategoryOnlyQuery)
{
    auto request = buildSearchRequest("", "Development");
    auto result = request->getUrl();
    QUrl expected("http://apps.nxos.org/api/applications/search?category=Development");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
    request->deleteLater();
}

TEST_F(TestRestClient, buildTextAndCategoryQuery)
{
    auto request = buildSearchRequest("appimage", "Development");
    auto result = request->getUrl();
    QUrl expected("http://apps.nxos.org/api/applications/search?query=appimage&category=Development");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
    request->deleteLater();
}

TEST_F(TestRestClient, buildGetApplicationUrl) {
    auto request = buildGetApplicationRequest("appimaged.desktop");
    auto result = request->getUrl();
    QUrl expected(
            R"(http://apps.nxos.org/api/applications/appimaged.desktop?filter={"include":[{"releases":{"files":{}}}]})");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
    request->deleteLater();
}


TEST_F(TestRestClient, getUnexistentApplication)
{
    auto request = buildGetApplicationRequest("_MISSING_APPLICATION_");
    QSignalSpy spy(request, &GetApplicationRequest::failed);
    request->start();

    spy.wait();
    ASSERT_EQ(1, spy.count());
    request->deleteLater();
}

TEST_F(TestRestClient, getExistentApplication)
{
    auto request = buildGetApplicationRequest("appimaged.desktop");
    QSignalSpy spy(request, &GetApplicationRequest::resultReady);
    request->start();

    spy.wait();
    ASSERT_EQ(1, spy.count());
    request->deleteLater();
}

TEST_F(TestRestClient, searchApplications)
{
    auto request = buildSearchRequest();
    QSignalSpy spy(request, &ApplicationsSearchRequest::resultsReady);
    request->start();

    spy.wait();
    ASSERT_EQ(1, spy.count());
    request->deleteLater();
}

}

#endif // NOMAD_SOFTWARE_CENTER_TESTRESTCLIENT_H