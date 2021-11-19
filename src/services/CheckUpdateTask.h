#pragma once
// system

// libraries
#include <QAppImageUpdate>
#include <QObject>

// local
#include "TaskManager.h"
#include "models/appsmodel.h"

class CheckUpdateTask : public Task
{
    Q_OBJECT
public:
    CheckUpdateTask(const QString &id, const QString &appImagePath, const QString &appName, AppsModel *appsModel, int index, QObject *parent);
    Q_SCRIPTABLE void start();

protected:
    Q_SLOT void onWorkerStarted(short);
    Q_SLOT void onWorkerFinished(QJsonObject info, short, AppsModel *appsModel, int index);
    Q_SLOT void onWorkerProgress(int, qint64, qint64, double, QString, short);
    Q_SLOT void onWorkerError(short, short);

private:
    QString _appName;

    QAppImageUpdate *_worker;
};
