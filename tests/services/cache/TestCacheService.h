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

    void testSaveApplicationWithoutBundles();
    void testListOneApplication();

    void cleanupTestCase();

private:
    CacheService *_service;
    QTemporaryDir _cacheDir;
    QString _cacheFile;

    ApplicationData _demoAppData;
};