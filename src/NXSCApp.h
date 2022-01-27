#pragma once
// system

// libraries
#include <KAboutData>
#include <QApplication>
#include <QGuiApplication>
#include <QPointer>
#include <QQmlApplicationEngine>
#include <QThread>

// local
#include "../tests/DummyProgressNotificationSource.h"
#include "models/ApplicationsRegistryModel.h"
#include "models/ApplicationsRegistryProxyModel.h"
#include "models/TasksListModel.h"
#include "services/db/AppsDBHelper.h"
#include "services/install/InstallService.h"
#include "services/registry/ApplicationsRegistry.h"
#include "services/registry/BundlesDirsWatcher.h"
#include "services/update/UpdateService.h"

class NXSCApp : public QApplication
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
    AppsDBHelper *_appsDBHelper;
    ApplicationsRegistry _applicationsRegistry;
    ApplicationsRegistryModel _applicationsRegistryModel;
    ApplicationsRegistryProxyModel _applicationsRegistryModelProxy;
    UpdateService _updateService;
    InstallService _installService;
    TasksListModel _tasksListModel;
    QPointer<BundlesDirsWatcher> _bundleDirsWatcher;
    QThread _bundleDirsWatcherThread;
    KAboutData _aboutData;
    QQmlApplicationEngine _engine;
    void setKDEApplicationData();
    void registerApplicationsRegistryService();
    void registerUpdateService();
    void registerThumbnailer();
    void setupApplicationDBUpdateCache();
};
