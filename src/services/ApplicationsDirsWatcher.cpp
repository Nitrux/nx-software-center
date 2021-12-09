#include "ApplicationsDirsWatcher.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

ApplicationsDirsWatcher::ApplicationsDirsWatcher(const QStringList &paths, QObject *parent)
    : QObject(parent)
    , _watcher()
{
    connect(&_watcher, &QFileSystemWatcher::directoryChanged, this, &ApplicationsDirsWatcher::onDirChanged);

    // paths need to be added after connection is made to avoid missing signals
    _watcher.addPaths(paths);
}

void ApplicationsDirsWatcher::onDirChanged(const QString &dirPath)
{
    checkCreationsAndUpdates(dirPath);
    checkDeletions(dirPath);
}

void ApplicationsDirsWatcher::checkCreationsAndUpdates(const QString &dirPath)
{
    QDir dir(dirPath);
    auto dirEntries = dir.entryList({QDir::Files});
    for (const auto &entry : dirEntries) {
        auto filePath = dir.absoluteFilePath(entry);
        QFileInfo fileInfo(filePath);
        auto lastModified = fileInfo.lastModified();

        if (_fileCache.contains(filePath)) {
            if (_fileCache[filePath] != lastModified) {
                emit(appimageUpdate(filePath));
            }
        } else
            emit(appimageAdded(filePath));

        _fileCache[filePath] = lastModified;
    }
}
void ApplicationsDirsWatcher::checkDeletions(const QString &dirPath)
{
    auto knownFiles = _fileCache.keys();
    for (const auto &path : knownFiles) {
        if (path.startsWith(dirPath) && !QFile::exists(path)) {
            _fileCache.remove(path);

            emit(appimageRemoved(path));
        }
    }
}
