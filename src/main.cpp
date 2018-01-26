#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QIcon>

#include <ui/UpgraderController.h>

#include "gateways/CacheSource.h"
#include "gateways/AppImageHubSource.h"
#include "gateways/SimpleDownloadManager.h"
#include "gateways/CachedDownloadManager.h"
#include "entities/Registry.h"
#include "entities/Executor.h"
#include "entities/Updater.h"
#include "entities/Cache.h"

#include "ui/SearchControler.h"
#include "ui/TasksController.h"
#include "ui/InstallController.h"
#include "ui/UninstallController.h"
#include "ui/RegistryController.h"
#include "ui/UpdaterController.h"

#define QML_MODULE_NAMESPACE "org.nxos.softwarecenter"
#define QML_MODULE_MAJOR_VERSION 1

Repository *repository;
DownloadManager *downloadManager = nullptr;
QNetworkAccessManager *networkAccessManager = nullptr;
Executor *executor = nullptr;
Registry *registry = nullptr;
Updater *updater = nullptr;
Cache *cache = nullptr;
Upgrader *upgrader = nullptr;


void registerQmlModules();
void initSoftwareCenterModules(QObject *parent);

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

#ifdef QT_DEBUG
    QQmlDebuggingEnabler enabler;
#endif

    QQmlApplicationEngine engine;


    app.setWindowIcon(QIcon::fromTheme("nx-software-center"));
    app.setApplicationDisplayName("Nomad Software Center");
    initSoftwareCenterModules(nullptr);
    registerQmlModules();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

void initSoftwareCenterModules(QObject *parent) {
    executor = new Executor();

    registry = new Registry();
    QObject::connect(executor, &Executor::taskCompleted, registry, &Registry::handleTaskCompleted);

    repository = new Repository();

    networkAccessManager = new QNetworkAccessManager(parent);
    downloadManager = new SimpleDownloadManager(networkAccessManager, parent);

    CacheSource *cacheSource = new CacheSource(Cache::getApplicationsCachePath(), parent);
    AppImageHubSource *appImageHubSource = new AppImageHubSource(downloadManager, parent);
    updater = new Updater(repository, {appImageHubSource, cacheSource});

    cache = new Cache;
    cache->setRepository(repository);
    QObject::connect(registry, &Registry::installedApplicationsChanged, cache, &Cache::handleInstalledApplicationsChanged);

    upgrader = new Upgrader();
    upgrader->setRepository(repository);
    upgrader->setInstalledApplications(registry->getInstalledApplications());
    QObject::connect(registry, &Registry::installedApplicationsChanged,
                     upgrader, &Upgrader::handleInstalledApplicationsChanged);
}



static QObject *searchControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    SearchControler *searchControler = new SearchControler(repository);
    return searchControler;
}

static QObject *tasksControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    TasksController *taskControler = new TasksController(executor);
    return taskControler;
}

static QObject *installControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    InstallController *installControler = new InstallController(repository, executor, downloadManager);
    return installControler;
}

static QObject *uninstallControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto *uninstallController = new UninstallController(repository, registry, executor);
    return uninstallController;
}

static QObject *registryControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    RegistryController *registryControler = new RegistryController(registry);
    return registryControler;
}

static QObject *updaterControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    UpdaterController *updaterController = new UpdaterController(updater);
    return updaterController;
}

static QObject *upgraderControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    UpgraderController *upgraderController = new UpgraderController(upgrader, repository, registry, executor, downloadManager);
    return upgraderController;
}

void registerQmlModules() {
    qmlRegisterSingletonType<SearchControler>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                              "SearchController",
                                              searchControllerSingletonProvider);


    qmlRegisterSingletonType<TasksController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                       "TasksController",
                                       tasksControllerSingletonProvider);

    qmlRegisterSingletonType<InstallController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                       "InstallController",
                                       installControllerSingletonProvider);

    qmlRegisterSingletonType<InstallController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                                "UninstallController",
                                                uninstallControllerSingletonProvider);

    qmlRegisterSingletonType<RegistryController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                       "RegistryController",
                                       registryControllerSingletonProvider);

    qmlRegisterSingletonType<UpdaterController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                       "UpdaterController",
                                       updaterControllerSingletonProvider);

    qmlRegisterSingletonType<UpgraderController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                       "UpgraderController",
                                       upgraderControllerSingletonProvider);
}
