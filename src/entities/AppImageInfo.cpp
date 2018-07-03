//
// Created by alexis on 7/3/18.
//

#include <iostream>
#include "AppImageInfo.h"
AppImageInfo AppImageInfo::fromVariant(const QVariant& variant)
{
    auto map = variant.toMap();
    if (!map.isEmpty()) {
        AppImageInfo a;
        a.id = map["id"].toString();
        a.name = LocalizedQString::fromVariant(map["name"]);
        a.icon = map["icon"].toString();
        a.abstract = LocalizedQString::fromVariant(map["abstract"]);
        a.description = LocalizedQString::fromVariant(map["description"]);
        a.license = License::fromVariant(map["license"]);
        a.categories = map["categories"].toStringList();
        a.keywords = map["keywords"].toStringList();
        a.languages = map["languages"].toStringList();
        a.developer = Developer::fromVariant(map["developer"]);
        a.release = Release::fromVariant(map["release"]);
        a.file = File::fromVariant(map["file"]);

        QList<RemoteImage> screenshots;
        for (const auto& v: map["screenshots"].toList()) {
            screenshots << RemoteImage::fromVariant(v);
        }
        a.screenshots = screenshots;

        a.mimeTypes = map["mime-types"].toStringList();

        QMap<QString, QString> links;
        auto linksVMap = map["links"].toMap();
        for (const auto& k: linksVMap.keys())
            links[k] = linksVMap[k].toString();

        a.links = links;
        return a;
    }
    return AppImageInfo();
}
QVariant AppImageInfo::toVariant(const AppImageInfo& appImageInfo)
{
    auto qStringListToQVariantList = [](const QStringList src) {
      QVariantList res;
      for (const auto& s: src)
          res << s;
      return res;
    };

    QVariantMap map;
    map["format"] = QVariant(1).toDouble();
    map["id"] = appImageInfo.id;
    map["name"] = LocalizedQString::toVariant(appImageInfo.name);
    map["icon"] = appImageInfo.icon;
    map["abstract"] = LocalizedQString::toVariant(appImageInfo.abstract);
    map["description"] = LocalizedQString::toVariant(appImageInfo.description);
    map["license"] = License::toVariant(appImageInfo.license);
    map["categories"] = qStringListToQVariantList(appImageInfo.categories);
    map["keywords"] = qStringListToQVariantList(appImageInfo.keywords);
    map["languages"] = qStringListToQVariantList(appImageInfo.languages);
    map["developer"] = Developer::toVariant(appImageInfo.developer);
    map["release"] = Release::toVariant(appImageInfo.release);
    map["file"] = File::toVariant(appImageInfo.file);

    QVariantList screenshots;
    for (const RemoteImage& image: appImageInfo.screenshots)
        screenshots << RemoteImage::toVariant(image);
    map["screenshots"] = screenshots;

    map["mime-types"] = qStringListToQVariantList(appImageInfo.mimeTypes);

    QVariantMap links;
    for (const auto& k: appImageInfo.links.keys())
        links[k] = appImageInfo.links[k];
    map["links"] = links;

    return map;
}
bool AppImageInfo::operator==(const AppImageInfo& rhs) const
{
    return id==rhs.id &&
            name==rhs.name &&
            icon==rhs.icon &&
            abstract==rhs.abstract &&
            description==rhs.description &&
            license==rhs.license &&
            categories==rhs.categories &&
            keywords==rhs.keywords &&
            languages==rhs.languages &&
            developer==rhs.developer &&
            release==rhs.release &&
            file==rhs.file &&
            screenshots==rhs.screenshots &&
            mimeTypes==rhs.mimeTypes &&
            links==rhs.links;
}
bool AppImageInfo::operator!=(const AppImageInfo& rhs) const
{
    return !(rhs==*this);
}

bool AppImageInfo::License::operator==(const AppImageInfo::License& rhs) const
{
    return id==rhs.id &&
            name==rhs.name &&
            body==rhs.body;
}
bool AppImageInfo::License::operator!=(const AppImageInfo::License& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const AppImageInfo::License& license)
{
    os << "{ id: " << license.id.toStdString() << " name: " << license.name.toStdString()
       << " body: " << license.body.toStdString() << " }";
    return os;
}
AppImageInfo::License AppImageInfo::License::fromVariant(QVariant& variant)
{
    License l;
    QVariantMap map = variant.toMap();
    l.id = map["id"].toString();
    l.name = map["name"].toString();
    l.body = map["body"].toString();
    return l;
}
QVariant AppImageInfo::License::toVariant(const AppImageInfo::License& license)
{
    QVariantMap map;
    map["id"] = license.id;
    map["name"] = license.name;
    map["body"] = license.body;
    return map;
}
bool AppImageInfo::Developer::operator==(const AppImageInfo::Developer& rhs) const
{
    return name==rhs.name &&
            avatar==rhs.avatar &&
            website==rhs.website &&
            pubkey==rhs.pubkey;
}
bool AppImageInfo::Developer::operator!=(const AppImageInfo::Developer& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const AppImageInfo::Developer& developer)
{
    os << "{ name: " << developer.name.toStdString() << " avatar: " << developer.avatar.toStdString()
       << " website: " << developer.website.toStdString() << " pubkey: " << developer.pubkey.toStdString() << " }";
    return os;
}
AppImageInfo::Developer AppImageInfo::Developer::fromVariant(QVariant& variant)
{
    auto map = variant.toMap();
    AppImageInfo::Developer developer;
    developer.name = map["name"].toString();
    developer.avatar = map["avatar"].toString();
    developer.website = map["website"].toString();
    developer.pubkey = map["pubkey"].toString();

    return developer;
}
QVariant AppImageInfo::Developer::toVariant(const AppImageInfo::Developer& developer)
{
    QVariantMap map;
    map["name"] = developer.name;
    map["avatar"] = developer.avatar;
    map["pubkey"] = developer.pubkey;
    map["website"] = developer.website;
    return map;
}
bool AppImageInfo::Release::operator==(const AppImageInfo::Release& rhs) const
{
    return date==rhs.date &&
            version==rhs.version &&
            channel==rhs.channel &&
            changelog==rhs.changelog;
}
bool AppImageInfo::Release::operator!=(const AppImageInfo::Release& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const AppImageInfo::Release& release)
{
    os << "{ date: " << release.date.toString().toStdString() << " version: " << release.version.toStdString()
       << " channel: " << release.channel.toStdString() << " changelog: " << release.changelog << " }";
    return os;
}
AppImageInfo::Release AppImageInfo::Release::fromVariant(const QVariant& variant)
{
    auto map = variant.toMap();
    AppImageInfo::Release release;
    release.version = map["version"].toString();
    release.date = QDateTime::fromString(map["date"].toString(), Qt::RFC2822Date);
    release.channel = map["channel"].toString();
    release.changelog = LocalizedQString::fromVariant(map["changelog"]);

    return release;
}
QVariant AppImageInfo::Release::toVariant(const AppImageInfo::Release& release)
{
    QVariantMap map;
    map["version"] = release.version;
    map["date"] = release.date.toString(/*Qt::RFC2822Date*/);
    map["channel"] = release.channel;
    map["changelog"] = LocalizedQString::toVariant(release.changelog);

    return map;
}
bool AppImageInfo::File::operator==(const AppImageInfo::File& rhs) const
{
    return type==rhs.type &&
            size==rhs.size &&
            architecture==rhs.architecture &&
            sha512checksum==rhs.sha512checksum &&
            remote_url==rhs.remote_url && local_url==rhs.local_url;
}
bool AppImageInfo::File::operator!=(const AppImageInfo::File& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const AppImageInfo::File& file)
{
    os << "{ type: " << file.type << " size: " << file.size
       << " architecture: " << file.architecture.toStdString()
       << " sha512checksum: " << file.sha512checksum.toStdString()
       << " remote_url: " << file.remote_url.toStdString() << " local_url: " << file.local_url.toStdString() << " }";
    return os;
}
AppImageInfo::File::File()
        :type(0), size(0) { }
AppImageInfo::File AppImageInfo::File::fromVariant(const QVariant& variant)
{
    auto map = variant.toMap();
    AppImageInfo::File file;
    file.type = map["type"].toInt();
    file.size = map["size"].toInt();
    file.architecture = map["architecture"].toString();
    file.sha512checksum = map["sha512checksum"].toString();
    file.local_url = map["local_url"].toString();
    file.remote_url = map["remote_url"].toString();

    return file;
}
QVariant AppImageInfo::File::toVariant(const AppImageInfo::File& file)
{
    QVariantMap map;
    map["type"] = QVariant(file.type).toDouble();
    map["size"] = QVariant(file.size).toDouble();
    map["architecture"] = file.architecture;
    map["sha512checksum"] = file.sha512checksum;
    map["remote_url"] = file.remote_url;
    map["local_url"] = file.local_url;

    return map;
}
bool AppImageInfo::RemoteImage::operator==(const AppImageInfo::RemoteImage& rhs) const
{
    return height==rhs.height &&
            width==rhs.width &&
            caption==rhs.caption &&
            url==rhs.url;
}
bool AppImageInfo::RemoteImage::operator!=(const AppImageInfo::RemoteImage& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const AppImageInfo::RemoteImage& image)
{
    os << "{ height: " << image.height << " width: " << image.width
       << " caption: " << image.caption.toStdString() << " url: " << image.url.toStdString() << " }";
    return os;
}
AppImageInfo::RemoteImage::RemoteImage()
        :height(0), width(0) { }
AppImageInfo::RemoteImage AppImageInfo::RemoteImage::fromVariant(const QVariant& variant)
{
    AppImageInfo::RemoteImage remoteImage;
    auto map = variant.toMap();
    remoteImage.url = map["url"].toString();
    remoteImage.caption = map["caption"].toString();
    remoteImage.height = map["height"].toInt();
    remoteImage.width = map["width"].toInt();
    remoteImage.language = map["language"].toString();
    return remoteImage;
}
QVariant AppImageInfo::RemoteImage::toVariant(const AppImageInfo::RemoteImage& image)
{
    QVariantMap map;
    map["height"] = QVariant(image.height).toDouble();
    map["width"] = QVariant(image.width).toDouble();
    map["language"] = image.language;
    map["caption"] = image.caption;
    map["url"] = image.url;

    return map;
}
std::ostream& operator<<(std::ostream& os, const AppImageInfo::LocalizedQString& string)
{
    os << "{ ";
    for (auto key: string.keys())
        os << key.toStdString() << ": " << string[key].toStdString() << ", ";
    os << " }";
    return os;
}
AppImageInfo::LocalizedQString AppImageInfo::LocalizedQString::fromVariant(const QVariant& variant)
{
    auto localizedStringVariant = variant.toMap();
    LocalizedQString localizedQString;
    for (const auto& k: localizedStringVariant.keys())
        localizedQString[k] = localizedStringVariant[k].toString();
    return localizedQString;
}
QVariant AppImageInfo::LocalizedQString::toVariant(const AppImageInfo::LocalizedQString& string)
{
    QVariantMap map;
    for (const auto& key: string.keys())
        map[key] = string[key];
    return map;
}
std::ostream& operator<<(std::ostream& os, const AppImageInfo& info)
{
    os << "id: " << info.id.toStdString() << " name: " << info.name
       << " icon: " << info.icon.toStdString() << " abstract: " << info.abstract
       << " description: " << info.description << " license: " << info.license
       << " categories: " << info.categories.join(", ").toStdString()
       << " keywords: " << info.keywords.join(", ").toStdString()
       << " languages: " << info.languages.join(", ").toStdString()
       << " developer: " << info.developer
       << " release: " << info.release << " file: " << info.file;
    os << " screenshots: ";
    for (const auto& screenshot: info.screenshots)
        os << screenshot << ", ";
    os << " mimeTypes: " << info.mimeTypes.join(", ").toStdString();
    os << " links: ";
    for (auto key: info.links.keys())
        os << key.toStdString() << ": " << info.links[key].toStdString() << ", ";
    return os;
}
