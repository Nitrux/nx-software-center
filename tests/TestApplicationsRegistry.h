#pragma once
// system

// libraries
#include <ApplicationsRegistry.h>
#include <QObject>
#include <QtTest/QtTest>

// local

class TestApplicationsRegistry : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void addBundle();

    void updateApplicationOnUpgrade();
    void updateApplicationOnDowngrade();

    void removeApplication();

    void cleanupTestCase();

private:
    ApplicationsRegistry *registry;
    ApplicationBundle *b1;
    ApplicationBundle *b2;
};
