#include <gtest/gtest.h>

#include <QFile>
#include <QDebug>
#include <QSignalSpy>
#include <QJsonDocument>
#include <entities/AppImageInfo.h>

namespace NX_SOFTWARE_CENTER_TESTS {
class TestAppImageInfo : public testing::Test {
public:
    static constexpr auto path = TEST_DATA_DIR"AppImageInfo.json";

    static QVariant loadTestVariant()
    {
        QFile f(path);
        QVariant variant;
        if (f.open(QIODevice::ReadOnly)) {
            auto data = f.readAll();
            auto doc = QJsonDocument::fromJson(data);
            variant = doc.toVariant();
        }
        return variant;
    }

    static AppImageInfo initTestAppImageInfo()
    {
        AppImageInfo a;
        a.id = "example-app.desktop";
        Application::LocalizedQString name;
        name["en"] = "Example App";
        name["es"] = "Aplicación de ejemplo";
        name["null"] = "this is used in the case that there is no locale info about this text";
        a.name = name;
        a.icon = "icon url";
        Application::LocalizedQString abstract;
        abstract["en"] = "Example Application Abstract.";
        abstract["es"] = "Resumen de la Applicación de Ejemplo.";
        a.abstract = abstract;
        Application::LocalizedQString description;
        description["en"] = "<p>English long description of example app</p>";
        description["es"] = "<p>Descripción en Español de la Aplicación de Ejemplo</p>";
        a.description = description;

        Application::License license;
        license.id = "gpl3";
        license.name = "GNU General Public License 3";
        license.body = "... GNU General Public License 3 ...";
        a.license = license;
        a.categories = QList<QString>{"Development"};
        a.keywords = QList<QString>{"application", "applicación", "example", "ejemplo"};
        a.languages = QList<QString>{"de_DE", "es_CU", "ru"};

        Application::Developer developer;
        developer.name = "Bob";
        developer.avatar = "https://bob.blog/logo.sng";
        developer.website = "https://github.com/AppImage";
        developer.pubkey = "gpg public key";
        a.developer = developer;

        Application::Release release;
        release.date = QDateTime::fromString("Tue, 26 Jun 2018 00:31:36 GMT", Qt::RFC2822Date);
        release.version = "0.1.0";
        release.channel = "stable";
        Application::LocalizedQString changelog;
        changelog["en"] = "<p>The application was created.</p>";
        changelog["es"] = "<p>La applicación fue creada.</p>";
        release.changelog = changelog;
        a.release = release;

        Application::File file;
        file.type = 2;
        file.size = 1882040;
        file.architecture = "x86-64";
        file.sha512checksum = "a big and fussy checksum";
        file.url = "https://github.com/AppImage/AppImageKit/releases/download/continuous/"
                          "appimagetool-x86_64.AppImage";
        a.file = file;

        Application::RemoteImage screenshot;
        screenshot.height = 0;
        screenshot.width = 0;
        screenshot.language = "";
        screenshot.caption = "Update from Ubuntu with AppImageUpdate";
        screenshot.url = "https://cloud.githubusercontent.com/assets/2480569/19410850/"
                         "0390fe9c-92f6-11e6-9882-3ca6d360a190.jpg";
        a.screenshots = {screenshot};
        a.mimeTypes = QList<QString>{"application/x-iso9660-image", "application/vnd.appimage"};

        QMap<QString, QString> links;
        links["homepage"] = "https://appimage.org";
        links["help"] = "https://github.com/AppImage/AppImageKit/wiki";
        links["donation"] = "https://www.paypal.me/azubieta";
        links["development"] = "https://github.com/AppImage/AppImageKit";
        a.links = links;
        return a;
    }
};

TEST_F(TestAppImageInfo, toVariant)
{
    auto src = TestAppImageInfo::initTestAppImageInfo();
    auto result = AppImageInfo::toVariant(src);
    auto expected = TestAppImageInfo::loadTestVariant();

    qInfo() << result;
    qInfo() << expected;
    ASSERT_EQ(result, expected);
}

TEST_F(TestAppImageInfo, fromVariant)
{
    QVariant variant = TestAppImageInfo::loadTestVariant();
    auto result = AppImageInfo::fromVariant(variant);
    auto expected = TestAppImageInfo::initTestAppImageInfo();

    ASSERT_EQ(result, expected);
}

TEST_F(TestAppImageInfo, toAndFromVariant) {
    auto a = TestAppImageInfo::initTestAppImageInfo();
    auto v = AppImageInfo::toVariant(a);
    auto a2 = AppImageInfo::fromVariant(v);

    ASSERT_EQ(a2, a);
}



}