#pragma once
// system

// libraries
#include <QFileInfo>
#include <QObject>
#include <QQueue>
#include <QSemaphore>
#include <QSettings>
#include <appimage/utils/ResourcesExtractor.h>

// local
#include "ApplicationBundle.h"

class BundleInspector
{
public:
    BundleInspector(const QString &filePath);
    QString getPath();
    ApplicationBundle getData();
    QDateTime getLastModified();
    QByteArray getMd5Checksum();

private:
    QFileInfo _fileInfo;
    ApplicationBundle _bundle;

    void extractDesktopEntryData(const appimage::utils::ResourcesExtractor &extractor);
    static QStringList extractXdgCategories(const QSettings &desktopEntry);
    void extractIcon(const appimage::utils::ResourcesExtractor &extractor);
};
