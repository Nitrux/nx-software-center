//
// Created by alexis on 3/6/18.
//

#include <QSignalSpy>
#include <QList>
#include <QFile>

#include <gtest/gtest.h>

#include <gateways/OCSStoreDataParser.h>


namespace NX_SOFTWARE_CENTER_TESTS {
    class TestOCSStoreDataParser : public testing::Test {
    public:
        OCSStoreDataParser parser;
    };

    TEST_F(TestOCSStoreDataParser, failure) {
        parser.setTarget(QUrl("http://wrong_ocs_url.com"));

        QSignalSpy spy(&parser, &OCSStoreDataParser::error);
        parser.extractApplications();
        spy.wait(100);

        ASSERT_EQ(spy.count(), 1);
    }

    TEST_F(TestOCSStoreDataParser, extractApps) {
        parser.setTarget(QUrl(TEST_DATA_DIR"appimagehub.com.data.xml"));


        QSignalSpy spy(&parser, &OCSStoreDataParser::completed);
        parser.extractApplications();


        ASSERT_EQ(1, spy.count());
        ASSERT_EQ(1, parser.getResults().size());

        Application a = parser.getResults().first();
        ASSERT_STREQ("Qtractor", a.getCodeName().toStdString().c_str());
        ASSERT_STREQ("0.8.6", a.getVersion().toStdString().c_str());

        const auto expectedDescription = "Qtractor is an audio/MIDI multi-track sequencer application written in C++ "
                "with the Qt framework. Target platform is Linux, where the Jack Audio Connection Kit (JACK) for "
                "audio and the Advanced Linux Sound Architecture (ALSA) for MIDI are the main "
                "infrastructures to evolve as a fairly-featured...";
        ASSERT_STREQ(expectedDescription,
                     a.getDescription().toStdString().c_str());

        ASSERT_EQ(QStringList({"rncbc"}), a.getAuthors());
        ASSERT_STREQ("https://dl.opendesktop.org/api/files/downloadfile/id/1517303596/s/"
                             "90efd5722b3b7360cd1dec08da008538/t/1520405941/o/1/qtractor-0.8.6-4.x86_64.AppImage",
                     a.getDownloadUrl().toStdString().c_str());

        ASSERT_EQ(10, a.getDownloadSize());
        ASSERT_EQ(QStringList(
                          {
                              "https://cn.pling.com/cache/770x540/img/5/3/4/3/17605c31d6e4915cad42c2a0a991da598d62.png",
                              "https://cn.pling.com/cache/770x540/img/8/2/0/f/0f52d3de9874c97ba25f3435de6e66187333.png"
                          }),
                  a.getScreenshots());

        ASSERT_STREQ("https://cn.pling.com/cache/100x100/img/5/3/4/3/17605c31d6e4915cad42c2a0a991da598d62.png",
                     a.getIcon().toStdString().c_str());
    }
}