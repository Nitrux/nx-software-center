#include "BundlesDirsWatcher.h"
#include "BundleInspector.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QThread>
#include <utility>

BundlesDirsWatcher::BundlesDirsWatcher(QObject *parent)
    : QObject(parent)
    , _watcher()
{
    connect(&_watcher, &QFileSystemWatcher::directoryChanged, this, &BundlesDirsWatcher::checkDirChanges);
}

void BundlesDirsWatcher::checkDirChanges(const QString &dirPath)
{
    checkCreationsAndUpdates(dirPath);
    checkDeletions(dirPath);
}

void BundlesDirsWatcher::checkCreationsAndUpdates(const QString &dirPath)
{
    QDir dir(dirPath);
    auto dirEntries = dir.entryList({QDir::Files});
    for (const auto &entry : dirEntries) {
        auto filePath = dir.absoluteFilePath(entry);
        auto inspector = BundleInspector(filePath);

        bool isInCache = _fileCache.contains(filePath);
        if (isInCache) {
            bool isCacheOutdated = _fileCache[filePath] != inspector.getLastModified();
            if (isCacheOutdated) {
                auto data = inspector.getData();
                if (data.bundleType == ApplicationBundle::AppImage)
                    emit(bundleUpdated(data));
            }
        } else {
            auto data = inspector.getData();
            if (data.bundleType == ApplicationBundle::AppImage)
                emit(bundleAdded(data));
        }

        // update cache
        _fileCache[filePath] = inspector.getLastModified();
    }
}

void BundlesDirsWatcher::checkDeletions(const QString &dirPath)
{
    auto knownFiles = _fileCache.keys();
    for (const auto &path : knownFiles) {
        if (path.startsWith(dirPath) && !QFile::exists(path)) {
            _fileCache.remove(path);

            emit(bundleRemoved(path));
        }
    }
}
void BundlesDirsWatcher::checkAllDirs()
{
    for (const auto &path : _watcher.directories())
        checkDirChanges(path);
}
void BundlesDirsWatcher::watchPaths(const QStringList &paths)
{
    for (const auto &path : paths)
        _watcher.addPath(path);
}

void BundlesDirsWatcher::setFileCache(QMap<QString, QDateTime> fileCache)
{
    _fileCache = std::move(fileCache);
}
