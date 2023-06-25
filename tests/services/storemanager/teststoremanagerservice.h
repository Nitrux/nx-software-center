#pragma once
// system

// libraries
#include <QObject>
#include <QSignalSpy>

// local
#include "services/storemanager/storemanagerservice.h"

class TestStoreManagerService : public QObject
{
    Q_OBJECT

    private slots:
        void initTestCase();

        void testGetCategories();
        void testGetApplications();
        void testGetAppimagehubApplications();

        void cleanupTestCase();

    private:
        StoreManagerService *_storeManagerService;

};