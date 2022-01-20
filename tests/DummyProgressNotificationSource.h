#pragma once
// system

// libraries
#include <QObject>
#include <QTimer>
// local
#include "services/TaskData.h"

class DummyProgressNotificationSource : public QObject
{
    Q_OBJECT
public:
    DummyProgressNotificationSource(QObject *parent = nullptr);
    Q_SIGNAL void progressNotification(const TaskData &update);

    QTimer timer;
    TaskData progress;
};
