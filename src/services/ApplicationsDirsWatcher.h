#pragma once
// system

// libraries
#include <QFileSystemWatcher>
#include <QMap>
#include <QObject>
#include <QStringList>

// local

class ApplicationsDirsWatcher : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationsDirsWatcher(const QStringList &paths, QObject *parent = nullptr);

    Q_SIGNAL void appimageAdded(QString path);
    Q_SIGNAL void appimageUpdate(QString path);
    Q_SIGNAL void appimageRemoved(QString path);

private:
    Q_SLOT void onDirChanged(const QString &dirPath);
    QFileSystemWatcher _watcher;

    // path - modification time
    QMap<QString, QDateTime> _fileCache;
    void checkCreationsAndUpdates(const QString &dirPath);
    void checkDeletions(const QString &path);
};
