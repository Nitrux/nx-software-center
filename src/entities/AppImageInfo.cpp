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
