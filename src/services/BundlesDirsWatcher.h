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
    explicit BundlesDirsWatcher(const QStringList &paths, QObject *parent = nullptr);

    Q_SIGNAL void bundleAdded(ApplicationBundle bundle);
    Q_SIGNAL void bundleUpdated(ApplicationBundle bundle);
    Q_SIGNAL void bundleRemoved(QString path);

private:
    Q_SLOT void onDirChanged(const QString &dirPath);

    QFileSystemWatcher _watcher;

    // path - modification time
    QMap<QString, QDateTime> _fileCache;
    void checkCreationsAndUpdates(const QString &dirPath);
    void checkDeletions(const QString &path);
    void checkNewFile(BundleInspector &inspector);
};
