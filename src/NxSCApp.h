#pragma once
// system

// libraries
#include <KAboutData>
#include <QGuiApplication>
#include <QPointer>
#include <QQmlApplicationEngine>
#include <QThread>

// local
#include "models/ApplicationsRegistryModel.h"
#include "models/ApplicationsRegistryProxyModel.h"
#include "services/ApplicationsRegistry.h"
#include "services/BundlesDirsWatcher.h"
#include "services/TaskManager.h"

class NxSCApp : public QGuiApplication
{
    Q_OBJECT
public:
    NxSCApp(int &argc, char **argv);
    void parseCommands();
    void setupQMLEngine();
    void setupApplicationsRegistry();
    ~NxSCApp() override;

protected:
    Q_SLOT void onQMLEngineObjectCreated(QObject *obj, const QUrl &objUrl);

private:
    QUrl _qml_main;
    TaskManager _taskManager;
    ApplicationsRegistry _applicationsRegistry;
    ApplicationsRegistryModel _applicationsRegistryModel;
    ApplicationsRegistryProxyModel _applicationsRegistryModelProxy;
    QPointer<BundlesDirsWatcher> _bundleDirsWatcher;
    QThread _bundleDirsWatcherThread;
    KAboutData _aboutData;
    QQmlApplicationEngine _engine;
    void setKDEApplicationData();
};
