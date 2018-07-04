#ifndef NOMAD_SOFTWARE_CENTER_TESTNAVIGATOR_H
#define NOMAD_SOFTWARE_CENTER_TESTNAVIGATOR_H

#include <gtest/gtest.h>
#include <QSignalSpy>
#include <gateways/ApplicationsSearchRequest.h>

#include "gateways/RestClient.h"

namespace NX_SOFTWARE_CENTER_TESTS {
class TestRestClient : public testing::Test, public RestClient {
public:
    TestRestClient()
            :RestClient("http://apps.nxos.org/api") { }
};

TEST_F(TestRestClient, buildEmptyQuery)
{
    auto result = buildSearchQueryUrl("", "");
    QUrl expected("http://localhost:3000/api/applications/search");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}

TEST_F(TestRestClient, buildTextOnlyQuery)
{
    auto result = buildSearchQueryUrl("appimage", "");
    QUrl expected("http://localhost:3000/api/applications/search?query=appimage");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}

TEST_F(TestRestClient, buildCategoryOnlyQuery)
{
    auto result = buildSearchQueryUrl("", "Development");
    QUrl expected("http://localhost:3000/api/applications/search?category=Development");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}

TEST_F(TestRestClient, buildTextAndCategoryQuery)
{
    auto result = buildSearchQueryUrl("appimage", "Development");
    QUrl expected("http://localhost:3000/api/applications/search?query=appimage&category=Development");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}

TEST_F(TestRestClient, fullSearchQuery)
{
    QSignalSpy spy(this, &RestClient::searchCompleted);
    search("appimage", "Development");

    spy.wait();
    ASSERT_EQ(1, spy.count());
}

TEST_F(TestRestClient, buildGetApplicationUrl) {
    auto result = buildGetApplicationUrl("appimaged.desktop");
    QUrl expected(
            R"(http://localhost:3000/api/applications/appimaged.desktop?filter={"include":[{"releases":{"files":{}}}]})");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}


TEST_F(TestRestClient, getUnexistentApp)
{
    QSignalSpy spy(this, &RestClient::failure);
    getApplication("dd");

    spy.wait();
    ASSERT_EQ(1, spy.count());
}

TEST_F(TestRestClient, getExistentApp)
{
    QSignalSpy spy(this, &RestClient::getApplicationCompleted);
    getApplication("appimaged.desktop");

    spy.wait();
    ASSERT_EQ(1, spy.count());
}

TEST_F(TestRestClient, buildSearchRequest)
{
    auto request = buildSearchRequest();
    QSignalSpy spy(request, &ApplicationsSearchRequest::resultsReady);
    request->start();

    spy.wait();
    ASSERT_EQ(1, spy.count());
}
}

#endif NOMAD_SOFTWARE_CENTER_TESTNAVIGATOR_H