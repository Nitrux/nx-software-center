//
// Created by alexis on 7/4/18.
//
#include <QDebug>
#include "AppImageInfo.h"

Application::LocalizedQString Application::LocalizedQString::fromVariant(const QVariant &variant) {
    auto localizedStringVariant = variant.toMap();
    LocalizedQString localizedQString;
    for (const auto &k: localizedStringVariant.keys())
        localizedQString[k] = localizedStringVariant[k].toString();
    return localizedQString;
}

QVariant Application::LocalizedQString::toVariant(const Application::LocalizedQString &string) {
    QVariantMap map;
    for (const auto &key: string.keys())
        map[key] = string[key];
    return map;
}

std::ostream &operator<<(std::ostream &os, const Application::LocalizedQString &string) {
    os << "[";
    for (const auto &item: string.keys())
        os << "{" << item.toStdString() << ": " << string[item].toStdString() << "}, ";
    os << "]";
    return os;
}

bool Application::License::operator==(const Application::License &rhs) const {
    return id == rhs.id &&
           name == rhs.name &&
           body == rhs.body;
}

bool Application::License::operator!=(const Application::License &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Application::License &license) {
    os << "{ id: " << license.id.toStdString() << " name: " << license.name.toStdString()
       << " body: " << license.body.toStdString() << " }";
    return os;
}

Application::License Application::License::fromVariant(const QVariant &variant) {
    License l;
    QVariantMap map = variant.toMap();
    l.id = map["id"].toString();
    l.name = map["name"].toString();
    l.body = map["body"].toString();
    return l;
}

QVariant Application::License::toVariant(const Application::License &license) {
    QVariantMap map;
    map["id"] = license.id;
    map["name"] = license.name;
    map["body"] = license.body;
    return map;
}

bool Application::Developer::operator==(const Application::Developer &rhs) const {
    return name == rhs.name &&
           avatar == rhs.avatar &&
           website == rhs.website &&
           pubkey == rhs.pubkey;
}

bool Application::Developer::operator!=(const Application::Developer &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Application::Developer &developer) {
    os << "{ name: " << developer.name.toStdString() << " avatar: " << developer.avatar.toStdString()
       << " website: " << developer.website.toStdString() << " pubkey: " << developer.pubkey.toStdString() << " }";
    return os;
}

Application::Developer Application::Developer::fromVariant(const QVariant &variant) {
    auto map = variant.toMap();
    Application::Developer developer;
    developer.name = map["name"].toString();
    developer.avatar = map["avatar"].toString();
    developer.website = map["website"].toString();
    developer.pubkey = map["pubkey"].toString();

    return developer;
}

QVariant Application::Developer::toVariant(const Application::Developer &developer) {
    QVariantMap map;
    map["name"] = developer.name;
    map["avatar"] = developer.avatar;
    map["pubkey"] = developer.pubkey;
    map["website"] = developer.website;
    return map;
}

bool Application::Release::operator==(const Application::Release &rhs) const {
    return date == rhs.date &&
           version == rhs.version &&
           channel == rhs.channel &&
           changelog == rhs.changelog;
}

bool Application::Release::operator!=(const Application::Release &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Application::Release &release) {
    os << "{ date: " << release.date.toString().toStdString() << " version: " << release.version.toStdString()
       << " channel: " << release.channel.toStdString() << " changelog: " << release.changelog << " }";
    return os;
}

Application::Release Application::Release::fromVariant(const QVariant &variant) {
    auto map = variant.toMap();
    Application::Release release;
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

QVariant Application::Release::toVariant(const Application::Release &release) {
    QVariantMap map;
    map["version"] = release.version;
    map["date"] = release.date.toString(/*Qt::RFC2822Date*/);
    map["channel"] = release.channel;
    map["changelog"] = LocalizedQString::toVariant(release.changelog);

    return map;
}

QList<Application::File> Application::Release::compatibleFiles(const QString &cpuArchitecture) const {
    QList<Application::File> result;
    for (const auto &file: files) {
        QString arch = file.architecture;
        if (arch.replace("-", "_") == cpuArchitecture)
            result << file;
    }

    return result;
}

bool Application::File::operator==(const Application::File &rhs) const {
    return type == rhs.type &&
           size == rhs.size &&
           architecture == rhs.architecture &&
           sha512checksum == rhs.sha512checksum &&
           url == rhs.url;
}

bool Application::File::operator!=(const Application::File &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Application::File &file) {
    os << "{ type: " << file.type << " size: " << file.size
       << " architecture: " << file.architecture.toStdString()
       << " sha512checksum: " << file.sha512checksum.toStdString()
       << " url: " << file.url.toStdString() << " }";
    return os;
}

Application::File::File()
        : type(0), size(0) {}

Application::File Application::File::fromVariant(const QVariant &variant) {
    auto map = variant.toMap();
    Application::File file;
    file.type = map["type"].toInt();
    file.size = map["size"].toInt();
    file.architecture = map["architecture"].toString();
    file.sha512checksum = map["sha512checksum"].toString();
    file.url = map["url"].toString();
    file.path = map["path"].toString();

    return file;
}

QVariant Application::File::toVariant(const Application::File &file) {
    QVariantMap map;
    map["type"] = QVariant(file.type).toDouble();
    map["size"] = QVariant(file.size).toDouble();
    map["architecture"] = file.architecture;
    map["sha512checksum"] = file.sha512checksum;
    map["url"] = file.url;
    map["path"] = file.path;

    return map;
}

bool Application::RemoteImage::operator==(const Application::RemoteImage &rhs) const {
    return height == rhs.height &&
           width == rhs.width &&
           caption == rhs.caption &&
           url == rhs.url;
}

bool Application::RemoteImage::operator!=(const Application::RemoteImage &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Application::RemoteImage &image) {
    os << "{ height: " << image.height << " width: " << image.width
       << " caption: " << image.caption.toStdString() << " url: " << image.url.toStdString() << " }";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Application &full) {
    auto printQStringList = [](std::ostream &os, const QList<QString> list) {
        for (const auto &item: list)
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
    for (const auto &item: full.releases) {
        os << item;
        const auto &files = item.files;
        os << "files: [";
        for (const auto &file: files)
            os << file << ", ";
        os << "]";
    }

    os << " screenshots: ";
    for (const auto &item: full.screenshots)
        os << item;

    os << " mimeTypes: ";
    printQStringList(os, full.mimeTypes);
    os << " links: ";
    for (const auto &link: full.links.keys())
        os << "{" << link.toStdString() << ": " << full.links[link].toStdString() << "}";
    os << std::endl;
    return os;
}

QVariant Application::toVariant() const {
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

Application::Release
Application::latestCompatibleRelease(const QString &cpuArchitecture, const QString &channel) {
    Application::Release latest;
    if (releases.size() > 0) {
        for (const auto &release: releases) {
            if ((!latest.date.isValid() || latest.date < release.date)
                && (channel.isEmpty() || release.channel == channel)
                && !release.compatibleFiles(cpuArchitecture).isEmpty())
                latest = release;
        }
    }

    return latest;
}

AppImageInfo Application::latestCompatibleReleaseInfo() {
    AppImageInfo info;
    info.id = id;
    info.name = name;
    info.abstract = abstract;
    info.icon = icon;
    info.description = description;
    info.license = license;
    info.categories = categories;
    info.keywords = keywords;
    info.languages = languages;
    info.developer = developer;
    info.screenshots = screenshots;
    info.mimeTypes = mimeTypes;
    info.links = links;
    info.release = latestCompatibleRelease(QSysInfo::currentCpuArchitecture());
    auto files = info.release.compatibleFiles(QSysInfo::currentCpuArchitecture());
    if (!files.empty())
        info.file = files.first();

    return info;
}

Application Application::from(const AppImageInfo &appImageInfo) {
    Application a;
    a.id = appImageInfo.id;
    a.name = appImageInfo.name;
    a.icon = appImageInfo.icon;
    a.abstract = appImageInfo.abstract;
    a.description = appImageInfo.description;
    a.license = appImageInfo.license;
    a.categories = appImageInfo.categories;
    a.keywords = appImageInfo.keywords;
    a.languages = appImageInfo.languages;
    a.developer = appImageInfo.developer;
    a.releases = {appImageInfo.release};
    a.screenshots = appImageInfo.screenshots;
    a.mimeTypes = appImageInfo.mimeTypes;
    a.links = appImageInfo.links;

    return a;
}

Application::RemoteImage::RemoteImage()
        : height(0), width(0) {}

Application::RemoteImage Application::RemoteImage::fromVariant(const QVariant &variant) {
    Application::RemoteImage remoteImage;
    auto map = variant.toMap();
    remoteImage.url = map["url"].toString();
    remoteImage.caption = map["caption"].toString();
    remoteImage.height = map["height"].toInt();
    remoteImage.width = map["width"].toInt();
    remoteImage.language = map["language"].toString();
    return remoteImage;
}

QVariant Application::RemoteImage::toVariant(const Application::RemoteImage &image) {
    QVariantMap map;
    map["height"] = QVariant(image.height).toDouble();
    map["width"] = QVariant(image.width).toDouble();
    map["language"] = image.language;
    map["caption"] = image.caption;
    map["url"] = image.url;

    return map;
}
