#include "BundleInspector.h"
#include "ApplicationData.h"

#include <QDebug>
#include <QSettings>
#include <QTemporaryFile>

#include <QStandardPaths>
#include <appimage/core/AppImage.h>
#include <appimage/utils/ResourcesExtractor.h>

ApplicationBundle BundleInspector::inspect(const QString &path)
{
    ApplicationBundle bundle(path);

    appimage::core::AppImage app(path.toStdString());
    appimage::utils::ResourcesExtractor extractor(app);

    extractDesktopEntryData(bundle, extractor);

    extractIcon(bundle, extractor);

    return bundle;
}
void BundleInspector::extractIcon(ApplicationBundle &bundle, const appimage::utils::ResourcesExtractor &extractor)
{
    QString iconPath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first() + "/icons/" + bundle.app->getId();
    extractor.extractTo({{".DirIcon", iconPath.toStdString()}});
    if (QFile::exists(iconPath))
        bundle.app->setIcon(iconPath);
}
void BundleInspector::extractDesktopEntryData(ApplicationBundle &bundle, const appimage::utils::ResourcesExtractor &extractor)
{
    auto desktopEntryPath = extractor.getDesktopEntryPath();
    bundle.app->setId(QString::fromStdString(desktopEntryPath.substr(0, desktopEntryPath.size() - 8)));

    QTemporaryFile temp;
    if (temp.open()) {
        extractor.extractTo({{desktopEntryPath, temp.fileName().toStdString()}});
        QSettings desktopEntry(temp.fileName(), QSettings::IniFormat);

        bundle.app->setName(desktopEntry.value("Desktop Entry/Name").toString());
        bundle.app->setDescription(desktopEntry.value("Desktop Entry/Comment").toString());

        QStringList xdgCategories = extractXdgCategories(desktopEntry);
        bundle.app->setXdgCategories(xdgCategories);

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
