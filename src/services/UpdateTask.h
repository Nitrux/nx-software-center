#pragma once
// system
#include <chrono>
#include <thread>

// libraries
#include <appimage/update.h>
#include <QObject>

// local
#include "TaskManager.h"

using namespace appimage::update;
using namespace std;

class UpdateTask : public Task
{
    Q_OBJECT
public:
    UpdateTask(const QString &id, const QString &appImagePath, const QString &appName, QObject *parent);
    Q_SCRIPTABLE void start();

private:
    QString _appName;

    Updater  *_worker;

    bool checkIfUpdateAvailable();
    void processUpdate();
};
