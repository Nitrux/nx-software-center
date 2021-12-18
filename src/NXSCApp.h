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
#include "services/update/UpdateService.h"

class NXSCApp : public QGuiApplication
{
    Q_OBJECT
public:
    NXSCApp(int &argc, char **argv);
    ~NXSCApp() override;

    void parseCommands();
    void setupQMLEngine();
    void setupApplicationsRegistry();

private:
    Q_SLOT void onQMLEngineObjectCreated(QObject *obj, const QUrl &objUrl);

    QUrl _qml_main;
    TaskManager _taskManager;
    ApplicationsRegistry _applicationsRegistry;
    ApplicationsRegistryModel _applicationsRegistryModel;
    ApplicationsRegistryProxyModel _applicationsRegistryModelProxy;
    UpdateService _updateService;
    QPointer<BundlesDirsWatcher> _bundleDirsWatcher;
    QThread _bundleDirsWatcherThread;
    KAboutData _aboutData;
    QQmlApplicationEngine _engine;
    void setKDEApplicationData();
    void registerApplicationsRegistryService();
    void registerUpdateService();
    void registerThumbnailer();
};