#pragma once
// system

// libraries
#include <QObject>
#include <QtTest/QtTest>

// local

class TestApplicationsRegistry : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void addBundle();

    void cleanupTestCase();
};
