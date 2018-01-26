#include <gtest/gtest.h>

#include <QSignalSpy>

#include "gateways/AppImageInstallLinksRegExParser.h"
#include "gateways/CachedDownloadManager.h"

namespace NX_SOFTWARE_CENTER_TESTS {
    class TestAppImageInstallLinksRegExParser : public testing::Test {
    protected:
        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
    };

    TEST_F(TestAppImageInstallLinksRegExParser, parseLinks) {
        QNetworkAccessManager networkAccessManager;
        CachedDownloadManager downloadManager(&networkAccessManager);
        AppImageInstallLinksRegExParser p("https://github.com/AlduinApp/alduin/releases", &downloadManager);

        static QStringList req_result;
        static QString req_error;
        QObject::connect(&p, &AppImageInstallLinksRegExParser::newLink, [=](const QString &link) {
            req_result << link;
        });
        QObject::connect(&p, &AppImageInstallLinksRegExParser::error, [=](const QString &error) {
            req_error = error;
        });
        QSignalSpy spy(&p, &AppImageInstallLinksRegExParser::finished);

        p.parse();

        spy.wait();

        if (req_error.isEmpty()) {
            ASSERT_EQ(1, spy.count());
            ASSERT_FALSE(req_result.isEmpty());
        } else
            qDebug() << req_error;

    }
}
