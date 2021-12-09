#pragma once
// system

// libraries
#include <QObject>
#include <QTest>

// local
#include "ApplicationsDirsWatcher.h"

class TestApplicationsDirsWatcher : public QObject
{
    Q_OBJECT
private:
    static void createFakeAppImage(const QString &path);

    Q_SLOT void initTestCase();
    Q_SLOT void cleanupTestCase();

    Q_SLOT void testWatchAppImageAdded();
    Q_SLOT void testWatchAppImageChanged();
    Q_SLOT void testWatchAppImageRemoved();

    static QString generateRandomString(int size);
    QDir dirToWatch;
    ApplicationsDirsWatcher *watcher;
};
