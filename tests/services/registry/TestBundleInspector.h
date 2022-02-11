#pragma once
// system

// libraries
#include <QObject>
#include <QtTest/QtTest>
#include <registry/ApplicationsRegistry.h>

// local

class TestBundleInspector : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void inspectBundle();

    void cleanupTestCase();
};
