#pragma once
// system

// libraries
#include <ApplicationsRegistry.h>
#include <QObject>
#include <QtTest/QtTest>

// local

class TestBundleInspector : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void inspectBundle();

    void cleanupTestCase();
};
