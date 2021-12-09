#pragma once
// system

// libraries
#include <QObject>
#include <QSettings>
#include <appimage/utils/ResourcesExtractor.h>

// local
#include "ApplicationBundle.h"

class BundleInspector : public QObject
{
    Q_OBJECT

public:
    static ApplicationBundle inspect(const QString &path);

private:
    static void extractDesktopEntryData(ApplicationBundle &bundle, const appimage::utils::ResourcesExtractor &extractor);
    static QStringList extractXdgCategories(const QSettings &desktopEntry);
    static void extractIcon(ApplicationBundle &bundle, const appimage::utils::ResourcesExtractor &extractor);
};
