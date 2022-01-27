#include "BundleInspector.h"
#include "services/ApplicationData.h"

#include <QDebug>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryFile>

#include <QCryptographicHash>
#include <appimage/core/AppImage.h>
#include <appimage/utils/ResourcesExtractor.h>

BundleInspector::BundleInspector(const QString &filePath)
    : _fileInfo(filePath)
    , _bundle(filePath)
{
}

ApplicationBundle BundleInspector::getData()
{
    _bundle.lastModified = _fileInfo.lastModified();
    _bundle.size = _fileInfo.size();
    try {
        appimage::core::AppImage app(_bundle.path.toStdString());
        appimage::utils::ResourcesExtractor extractor(app);
        extractDesktopEntryData(extractor);

        _bundle.bundleType = ApplicationBundle::AppImage;
//        extractIcon(extractor);
//        _bundle.hashSumMD5 = getMd5Checksum();
    } catch (std::runtime_error &error) {
        // report this as an unknown type bundle in case of error
        _bundle.bundleType = ApplicationBundle::Unknown;
    }

    return _bundle;
}

void BundleInspector::extractIcon(const appimage::utils::ResourcesExtractor &extractor)
{
    QString iconPath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first() + "/icons/" + _bundle.app->getId();
    extractor.extractTo({{".DirIcon", iconPath.toStdString()}});
    if (QFile::exists(iconPath))
        _bundle.app->setIcon(iconPath);
}
void BundleInspector::extractDesktopEntryData(const appimage::utils::ResourcesExtractor &extractor)
{
    auto desktopEntryPath = extractor.getDesktopEntryPath();
    _bundle.app->setId(QString::fromStdString(desktopEntryPath.substr(0, desktopEntryPath.size() - 8)));

    QTemporaryFile temp;
    if (temp.open()) {
        extractor.extractTo({{desktopEntryPath, temp.fileName().toStdString()}});
        QSettings desktopEntry(temp.fileName(), QSettings::IniFormat);

        _bundle.app->setName(desktopEntry.value("Desktop Entry/Name").toString());
        _bundle.app->setDescription(desktopEntry.value("Desktop Entry/Comment").toString());
        _bundle.app->setVersion(desktopEntry.value("Desktop Entry/X-AppImage-Version", QString()).toString());
        _bundle.app->setRequiresTerminal(desktopEntry.value("Desktop Entry/Terminal", false).toBool());

        QStringList xdgCategories = extractXdgCategories(desktopEntry);
        _bundle.app->setXdgCategories(xdgCategories);

        temp.close();
    } else {
        throw std::runtime_error("BundleInspector was unable to create temporary file");
    }
}
QStringList BundleInspector::extractXdgCategories(const QSettings &desktopEntry)
{
    QStringList xdgCategories;
    for (auto cat : desktopEntry.value("Desktop Entry/Categories").toString().split(";")) {
        cat = cat.trimmed();
        if (cat.length() > 0)
            xdgCategories.push_back(cat);
    }
    return xdgCategories;
}

QDateTime BundleInspector::getLastModified()
{
    return _fileInfo.lastModified();
}

QByteArray BundleInspector::getMd5Checksum()
{
    if (!_bundle.hashSumMD5.isEmpty())
        return _bundle.hashSumMD5;

    QFile file(_bundle.path);
    if (!file.open(QIODevice::ReadOnly))
        return {};

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(&file);

    _bundle.hashSumMD5 = hash.result();
    return _bundle.hashSumMD5;
}
QString BundleInspector::getPath()
{
    return _bundle.path;
}
