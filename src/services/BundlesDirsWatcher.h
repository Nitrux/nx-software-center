#pragma once
// system

// libraries
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QMap>
#include <QObject>
#include <QStringList>

// local
#include "ApplicationBundle.h"
#include "BundleInspector.h"

class BundlesDirsWatcher : public QObject
{
    Q_OBJECT
public:
    explicit BundlesDirsWatcher(const QStringList &paths, QMap<QString, QDateTime> fileCache = {}, QObject *parent = nullptr);

    Q_SIGNAL void bundleAdded(ApplicationBundle bundle);
    Q_SIGNAL void bundleUpdated(ApplicationBundle bundle);
    Q_SIGNAL void bundleRemoved(QString path);

    Q_SLOT void checkAllDirs();

private:
    Q_SLOT void checkDirChanges(const QString &dirPath);

    QFileSystemWatcher _watcher;

    // path - modification time
    QMap<QString, QDateTime> _fileCache;
    void checkCreationsAndUpdates(const QString &dirPath);
    void checkDeletions(const QString &path);
};
