#pragma once
// system

// libraries
#include <appimage/update.h>

// local
#include "Task.h"
#include <QThread>
#include <QTimer>
#include <models/appsmodel.h>
#include "utils/appimagetools.h"

using namespace std;

class UpdateAllTask : public Task
{
    Q_OBJECT

public:
    UpdateAllTask(const QString &id, AppsModel *appsModel, TaskManager *taskManager);

    Q_SCRIPTABLE void start() override;

private:
    Q_SLOT void onUpdateCompleted();

private:
    QThread *_worker;
    AppsModel *_appsModel;
    TaskManager *_taskManager;

    FMH::MODEL_LIST getUpdatableApps() const;
    int updateApps(const QVector<FMH::MODEL> &apps);
};