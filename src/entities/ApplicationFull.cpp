//
// Created by alexis on 7/4/18.
//
#include <QDebug>
#include "AppImageInfo.h"
#include "ApplicationFull.h"

ApplicationFull::LocalizedQString ApplicationFull::LocalizedQString::fromVariant(const QVariant& variant)
{
    auto localizedStringVariant = variant.toMap();
    LocalizedQString localizedQString;
    for (const auto& k: localizedStringVariant.keys())
        localizedQString[k] = localizedStringVariant[k].toString();
    return localizedQString;
}
QVariant ApplicationFull::LocalizedQString::toVariant(const ApplicationFull::LocalizedQString& string)
{
    QVariantMap map;
    for (const auto& key: string.keys())
        map[key] = string[key];
    return map;
}
std::ostream& operator<<(std::ostream& os, const ApplicationFull::LocalizedQString& string)
{
    os << "[";
    for (const auto& item: string.keys())
        os << "{" << item.toStdString() << ": " << string[item].toStdString() << "}, ";
    os << "]";
    return os;
}
bool ApplicationFull::License::operator==(const ApplicationFull::License& rhs) const
{
    return id==rhs.id &&
            name==rhs.name &&
            body==rhs.body;
}
bool ApplicationFull::License::operator!=(const ApplicationFull::License& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const ApplicationFull::License& license)
{
    os << "{ id: " << license.id.toStdString() << " name: " << license.name.toStdString()
       << " body: " << license.body.toStdString() << " }";
    return os;
}
ApplicationFull::License ApplicationFull::License::fromVariant(const QVariant& variant)
{
    License l;
    QVariantMap map = variant.toMap();
    l.id = map["id"].toString();
    l.name = map["name"].toString();
    l.body = map["body"].toString();
    return l;
}
QVariant ApplicationFull::License::toVariant(const ApplicationFull::License& license)
{
    QVariantMap map;
    map["id"] = license.id;
    map["name"] = license.name;
    map["body"] = license.body;
    return map;
}
bool ApplicationFull::Developer::operator==(const ApplicationFull::Developer& rhs) const
{
    return name==rhs.name &&
            avatar==rhs.avatar &&
            website==rhs.website &&
            pubkey==rhs.pubkey;
}
bool ApplicationFull::Developer::operator!=(const ApplicationFull::Developer& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const ApplicationFull::Developer& developer)
{
    os << "{ name: " << developer.name.toStdString() << " avatar: " << developer.avatar.toStdString()
       << " website: " << developer.website.toStdString() << " pubkey: " << developer.pubkey.toStdString() << " }";
    return os;
}
ApplicationFull::Developer ApplicationFull::Developer::fromVariant(const QVariant& variant)
{
    auto map = variant.toMap();
    ApplicationFull::Developer developer;
    developer.name = map["name"].toString();
    developer.avatar = map["avatar"].toString();
    developer.website = map["website"].toString();
    developer.pubkey = map["pubkey"].toString();

    return developer;
}
QVariant ApplicationFull::Developer::toVariant(const ApplicationFull::Developer& developer)
{
    QVariantMap map;
    map["name"] = developer.name;
    map["avatar"] = developer.avatar;
    map["pubkey"] = developer.pubkey;
    map["website"] = developer.website;
    return map;
}
bool ApplicationFull::Release::operator==(const ApplicationFull::Release& rhs) const
{
    return date==rhs.date &&
            version==rhs.version &&
            channel==rhs.channel &&
            changelog==rhs.changelog;
}
bool ApplicationFull::Release::operator!=(const ApplicationFull::Release& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const ApplicationFull::Release& release)
{
    os << "{ date: " << release.date.toString().toStdString() << " version: " << release.version.toStdString()
       << " channel: " << release.channel.toStdString() << " changelog: " << release.changelog << " }";
    return os;
}
ApplicationFull::Release ApplicationFull::Release::fromVariant(const QVariant& variant)
{
    auto map = variant.toMap();
    ApplicationFull::Release release;
    release.version = map["version"].toString();

    release.date = map["date"].toDateTime();
    if (!release.date.isValid())
        release.date = QDateTime::fromString(map["date"].toString(), Qt::ISODate);
    if (!release.date.isValid())
        release.date = QDateTime::fromString(map["date"].toString(), Qt::RFC2822Date);

    release.channel = map["channel"].toString();
    release.changelog = LocalizedQString::fromVariant(map["changelog"]);

    return release;
}
QVariant ApplicationFull::Release::toVariant(const ApplicationFull::Release& release)
{
    QVariantMap map;
    map["version"] = release.version;
    map["date"] = release.date.toString(/*Qt::RFC2822Date*/);
    map["channel"] = release.channel;
    map["changelog"] = LocalizedQString::toVariant(release.changelog);

    return map;
}
QList<ApplicationFull::File> ApplicationFull::Release::compatibleFiles(const QString& cpuArchitecture) const
{
    QList<ApplicationFull::File> result;
    for (const auto& file: files) {
        QString arch = file.architecture;
        if (arch.replace("-", "_")==cpuArchitecture)
            result << file;
    }

    return result;
}
bool ApplicationFull::File::operator==(const ApplicationFull::File& rhs) const
{
    return type==rhs.type &&
            size==rhs.size &&
            architecture==rhs.architecture &&
            sha512checksum==rhs.sha512checksum &&
            url==rhs.url;
}
bool ApplicationFull::File::operator!=(const ApplicationFull::File& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const ApplicationFull::File& file)
{
    os << "{ type: " << file.type << " size: " << file.size
       << " architecture: " << file.architecture.toStdString()
       << " sha512checksum: " << file.sha512checksum.toStdString()
       << " url: " << file.url.toStdString() << " }";
    return os;
}
ApplicationFull::File::File()
        :type(0), size(0) { }

ApplicationFull::File ApplicationFull::File::fromVariant(const QVariant& variant)
{
    auto map = variant.toMap();
    ApplicationFull::File file;
    file.type = map["type"].toInt();
    file.size = map["size"].toInt();
    file.architecture = map["architecture"].toString();
    file.sha512checksum = map["sha512checksum"].toString();
    file.url = map["url"].toString();
    file.path = map["path"].toString();

    return file;
}
QVariant ApplicationFull::File::toVariant(const ApplicationFull::File& file)
{
    QVariantMap map;
    map["type"] = QVariant(file.type).toDouble();
    map["size"] = QVariant(file.size).toDouble();
    map["architecture"] = file.architecture;
    map["sha512checksum"] = file.sha512checksum;
    map["url"] = file.url;
    map["path"] = file.path;

    return map;
}
bool ApplicationFull::RemoteImage::operator==(const ApplicationFull::RemoteImage& rhs) const
{
    return height==rhs.height &&
            width==rhs.width &&
            caption==rhs.caption &&
            url==rhs.url;
}
bool ApplicationFull::RemoteImage::operator!=(const ApplicationFull::RemoteImage& rhs) const
{
    return !(rhs==*this);
}
std::ostream& operator<<(std::ostream& os, const ApplicationFull::RemoteImage& image)
{
    os << "{ height: " << image.height << " width: " << image.width
       << " caption: " << image.caption.toStdString() << " url: " << image.url.toStdString() << " }";
    return os;
}
std::ostream& operator<<(std::ostream& os, const ApplicationFull& full)
{
    auto printQStringList = [](std::ostream& os, const QList<QString> list) {
      for (const auto& item: list)
          os << item.toStdString();
    };
    os << "id: " << full.id.toStdString();
    os << " name: " << full.name;
    os << " icon: " << full.icon.toStdString();
    os << " abstract: " << full.abstract << " description: " << full.description << " license: " << full.license;
    os << " categories: ";
    printQStringList(os, full.categories);

    os << " keywords: ";
    printQStringList(os, full.keywords);

    os << " languages: ";
    printQStringList(os, full.languages);

    os << " developer: " << full.developer;

    os << " releases: ";
    for (const auto& item: full.releases) {
        os << item;
        const auto& files = item.files;
        os << "files: [";
        for (const auto& file: files)
            os << file << ", ";
        os << "]";
    }

    os << " screenshots: ";
    for (const auto& item: full.screenshots)
        os << item;

    os << " mimeTypes: ";
    printQStringList(os, full.mimeTypes);
    os << " links: ";
    for (const auto& link: full.links.keys())
        os << "{" << link.toStdString() << ": " << full.links[link].toStdString() << "}";
    os << std::endl;
    return os;
}
QVariant ApplicationFull::toVariant() const
{
    QVariantMap map;
    map["id"] = id;
    map["name"] = LocalizedQString::toVariant(name);
    map["icon"] = icon;
    map["abstract"] = LocalizedQString::toVariant(abstract);
    map["description"] = LocalizedQString::toVariant(description);
    map["license"] = License::toVariant(license);
    map["categories"] = categories;
    map["keywords"] = keywords;
    map["languages"] = languages;
    map["developer"] = Developer::toVariant(developer);
    QVariantList releasesVList;
    for (const auto &release: releases)
        releasesVList << Release::toVariant(release);
    map["releases"] = releasesVList;

    QVariantList screenshotsVList;
    for (const auto &screenshot: screenshots)
        screenshotsVList << RemoteImage::toVariant(screenshot);
    map["releases"] = screenshotsVList;
    map["mime-type"] = mimeTypes;
    QVariantMap linksVmap;
    for (const auto &key: links)
        linksVmap[key] = links[key];
    map["links"] = linksVmap;

    return map;
}

ApplicationFull::Release
ApplicationFull::latestCompatibleRelease(const QString& cpuArchitecture, const QString& channel)
{
    ApplicationFull::Release latest;
    if (releases.size()>0) {
        for (const auto& release: releases) {
            if ((!latest.date.isValid() || latest.date<release.date)
                    && (channel.isEmpty() || release.channel==channel)
                    && !release.compatibleFiles(cpuArchitecture).isEmpty())
                latest = release;
        }
    }

    return latest;
}
ApplicationFull::RemoteImage::RemoteImage()
        :height(0), width(0) { }

ApplicationFull::RemoteImage ApplicationFull::RemoteImage::fromVariant(const QVariant& variant)
{
    ApplicationFull::RemoteImage remoteImage;
    auto map = variant.toMap();
    remoteImage.url = map["url"].toString();
    remoteImage.caption = map["caption"].toString();
    remoteImage.height = map["height"].toInt();
    remoteImage.width = map["width"].toInt();
    remoteImage.language = map["language"].toString();
    return remoteImage;
}
QVariant ApplicationFull::RemoteImage::toVariant(const ApplicationFull::RemoteImage& image)
{
    QVariantMap map;
    map["height"] = QVariant(image.height).toDouble();
    map["width"] = QVariant(image.width).toDouble();
    map["language"] = image.language;
    map["caption"] = image.caption;
    map["url"] = image.url;

    return map;
}
