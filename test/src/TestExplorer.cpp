#ifndef NOMAD_SOFTWARE_CENTER_TESTNAVIGATOR_H
#define NOMAD_SOFTWARE_CENTER_TESTNAVIGATOR_H

#include <gtest/gtest.h>
#include <QSignalSpy>

#include "entities/Explorer.h"

namespace NX_SOFTWARE_CENTER_TESTS {
class TestExplorer : public testing::Test, public Explorer {
public:
    TestExplorer()
            :Explorer("http://localhost:3000/api") { }
};

TEST_F(TestExplorer, buildEmptyQuery)
{
    auto result = buildSearchQueryUrl("", "");
    QUrl expected("http://localhost:3000/api/applications/search");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}

TEST_F(TestExplorer, buildTextOnlyQuery)
{
    auto result = buildSearchQueryUrl("appimage", "");
    QUrl expected("http://localhost:3000/api/applications/search?query=appimage");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}

TEST_F(TestExplorer, buildCategoryOnlyQuery)
{
    auto result = buildSearchQueryUrl("", "Development");
    QUrl expected("http://localhost:3000/api/applications/search?category=Development");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}

TEST_F(TestExplorer, buildTextAndCategoryQuery)
{
    auto result = buildSearchQueryUrl("appimage", "Development");
    QUrl expected("http://localhost:3000/api/applications/search?query=appimage&category=Development");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}

TEST_F(TestExplorer, fullSearchQuery)
{
    QSignalSpy spy(this, &Explorer::searchCompleted);
    search("appimage", "Development");

    spy.wait();
    ASSERT_EQ(1, spy.count());
}

TEST_F(TestExplorer, buildGetApplicationUrl) {
    auto result = buildGetApplicationUrl("appimaged.desktop");
    QUrl expected(
            R"(http://localhost:3000/api/applications/appimaged.desktop?filter={"include":[{"releases":{"files":{}}}]})");
    ASSERT_EQ(expected.toEncoded(), result.toEncoded());
}


TEST_F(TestExplorer, getUnexistentApp)
{
    QSignalSpy spy(this, &Explorer::failure);
    getApplication("dd");

    spy.wait();
    ASSERT_EQ(1, spy.count());
}

TEST_F(TestExplorer, getExistentApp)
{
    QSignalSpy spy(this, &Explorer::getApplicationCompleted);
    getApplication("appimaged.desktop");

    spy.wait();
    ASSERT_EQ(1, spy.count());
}
}

#endif NOMAD_SOFTWARE_CENTER_TESTNAVIGATOR_H