#pragma once
// system

// libraries
#include <QObject>
#include <QTest>


// local

class TestApplicationData : public QObject
{
    Q_OBJECT

private:
    Q_SLOT void testAddBundle();
    Q_SLOT void createFromBundle();
};
