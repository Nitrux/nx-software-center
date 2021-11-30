#pragma once
// system

// libraries
#include <appimage/update.h>

// local
#include "Task.h"
#include <QThread>
#include <QTimer>
#include <models/appsmodel.h>

class CheckUpdateBulkTask : public Task
{
    Q_OBJECT

public:
    CheckUpdateBulkTask(const QString &id, AppsModel *appsModel, TaskManager *taskManager);

    Q_SCRIPTABLE void start() override;

private:
    Q_SLOT QSet<QString> getUpdatableApps(const QVector<FMH::MODEL> &apps);
    Q_SLOT void onCheckCompleted();

private:
    QThread *_worker;
    AppsModel *_appsModel;
    TaskManager *_taskManager;
    FMH::MODEL_LIST getAppSortedByName() const;
    void updateUpdatablePropertyOnModel(const QSet<QString> &updatable);
};
