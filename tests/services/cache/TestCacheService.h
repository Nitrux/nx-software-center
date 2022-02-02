#pragma once
// system

// libraries
#include <QObject>
#include <QTemporaryDir>

// local

class TestCacheService : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testInitDB();

    void testSaveApplication();

    void cleanupTestCase();

private:
    QTemporaryDir _cacheDir;
    QString _cacheFile;
};