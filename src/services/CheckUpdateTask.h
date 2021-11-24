#pragma once
// system
#include <chrono>
#include <thread>

// libraries
#include <appimage/update.h>
#include <QObject>

// local
#include "TaskManager.h"
#include "models/appsmodel.h"

using namespace appimage::update;
using namespace std;

class CheckUpdateTask : public Task
{
    Q_OBJECT
public:
    CheckUpdateTask(const QString &id, const QString &appImagePath, const QString &appName, AppsModel *appsModel, int index, QObject *parent);
    Q_SCRIPTABLE void start();

private:
    QString _appName;

    Updater *_worker;
    TaskManager *_taskManager;
    AppsModel *_appsModel;
    int _index;

    void processUpdate(AppsModel *appsModel, int index);
};
