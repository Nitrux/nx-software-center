//
// Created by alexis on 25/01/18.
//

#include "entities/Application.h"
#include "entities/ApplicationSerializer.h"

#include <gtest/gtest.h>

namespace NX_SOFTWARE_CENTER_TESTS {

    class TestApplicationSerializer : public testing::Test {
    protected:
        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
    };

    TEST_F(TestApplicationSerializer, jsonSerialization) {
        Application a("app", "1.0.1");
        a.setName("Application");
        a.setDescription("A very nice and useful application");
        a.setLicence("GPLv3");
        a.setHomePageUrl("http://nowere.com/app_1.0.0");
        a.setDownloadUrl("http://nowere.com/app/app_1.0.0.AppImage");
        a.setScreenshots({"http://nowere.com/app/screenshot1.png",
                          "http://nowere.com/app/screenshot2.png"});
        a.setCategories({"Utils", "System"});
        a.setAuthors({"Me", "My Self", "I"});
        a.setArch("amd64");


        QByteArray output = ApplicationSerializer::serialize(a);

        Application b = ApplicationSerializer::deserialize(output);

        ASSERT_EQ(a, b);
    }

}