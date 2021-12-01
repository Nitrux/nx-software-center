#pragma once
// system
#include <chrono>
#include <thread>

// libraries
#include <appimage/update.h>
#include <QObject>

// local
#include "TaskManager.h"
#include "utils/appimagetools.h"

using namespace appimage::update;
using namespace std;

class UpdateTask : public Task
{
    Q_OBJECT
public:
    UpdateTask(const QString &id, const QString &appImagePath, const QString &appName, TaskManager *taskManager);
    Q_SCRIPTABLE void start() override;

private:
    QString _appName;

    Updater  *_worker;
    TaskManager *_taskManager;

    bool checkIfUpdateAvailable();
    void processUpdate();
};
