#pragma once
// system

// libraries
#include <QObject>
#include <QTest>
#include <QThread>

// local
#include "BundlesDirsWatcher.h"

class TestBundlesDirsWatcher : public QObject
{
    Q_OBJECT
private:
    static void copyAppImage(const QString &path);

    Q_SLOT void initTestCase();
    Q_SLOT void cleanupTestCase();

    Q_SLOT void testWatchBundleCacheInitialization();
    Q_SLOT void testWatchBundleAdded();
    Q_SLOT void testWatchBundleChanged();
    Q_SLOT void testWatchBundleRemoved();

    static QString generateRandomString(int size);
    QDir dirToWatch;
    BundlesDirsWatcher *watcher;
    QThread _workerThread;
};
