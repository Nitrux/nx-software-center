#pragma once
// system

// libraries
#include <QAppImageUpdate>
#include <QObject>

// local
#include "TaskManager.h"

class UpdateTask : public Task
{
    Q_OBJECT
public:
    UpdateTask(const QString &id, const QString &appImagePath, const QString &appName, QObject *parent);
    Q_SCRIPTABLE void start();

protected:
    Q_SLOT void onWorkerStarted(short);
    Q_SLOT void onWorkerFinished(QJsonObject info, short);
    Q_SLOT void onWorkerProgress(int, qint64, qint64, double, QString, short);
    Q_SLOT void onWorkerError(short, short);

private:
    QString _appName;

    QAppImageUpdate *_worker;
};
