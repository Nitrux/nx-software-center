#pragma once
// system

// libraries
#include <QObject>
#include <QTemporaryDir>

// local
#include "services/cache/CacheService.h"

class TestCacheService : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testInitDB();
    void testSaveApplications();
    void testListApplication();
    void testModifyApplication();
    void testRemoveApplication();

    void cleanupTestCase();

private:
    CacheService *_service;
    QTemporaryDir _cacheDir;
    QString _cacheFile;

    ApplicationBundle _demoAppBundles[2];
    ApplicationBundle _otherAppBundles[2];
    void fillBundleData(int version, const QString &appName, ApplicationBundle &bundle) const;
};