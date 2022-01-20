#pragma once
// system

// libraries
#include <QObject>
#include <QTest>
#include <QThread>

// local
#include "services/update/UpdateService.h"

class TestUpdateService : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void initTestCase();
    Q_SLOT void cleanupTestCase();

    Q_SLOT void testCheckUpdatesOnAppWithoutBundles();
    Q_SLOT void testCheckUpdates();
    Q_SLOT void testCheckUpdate();
    QString _outdatedAppImagePath;
    QDir _tempdir;
    UpdateService service;
};
