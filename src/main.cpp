#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QIcon>

#include <ui/UpgraderController.h>
#include <ui/NotificationsController.h>
#include <ui/DeployedApplicationsController.h>
#include <QtCore/QStandardPaths>

#include "entities/Updater.h"
#include "entities/Cache.h"
#include "gateways/CacheSource.h"

#include "ui/SearchController.h"
#include "ui/TasksController.h"
#include "ui/DeployController.h"
#include "ui/RegistryController.h"
#include "ui/UpdaterController.h"
#include "ui/RunController.h"
#include "ui/ApplicationViewController.h"

#define QML_MODULE_NAMESPACE "org.nxos.softwarecenter"
#define QML_MODULE_MAJOR_VERSION 1

ApplicationRepository *repository;
QNetworkAccessManager *networkAccessManager = nullptr;
Executor *executor = nullptr;
Worker *worker = nullptr;
Deployer *installer = nullptr;
Registry *registry = nullptr;
Updater *updater = nullptr;
Cache *cache = nullptr;
Upgrader *upgrader = nullptr;
DeployedApplicationsRegistry *deployedApplicationsRegistry = nullptr;

void registerQmlModules();

void initSoftwareCenterModules(QObject *parent);

Q_DECLARE_METATYPE(Application)

Q_DECLARE_METATYPE(QList<Application>)

Q_DECLARE_METATYPE(UpgradeList)

void registerMetatypes();

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
    registerMetatypes();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

void registerMetatypes() {
    qRegisterMetaType<Application>("Application");
    qRegisterMetaType<QList<Application>>("ApplicationList");
    qRegisterMetaType<UpgradeList>("UpgradeList");
}

void initSoftwareCenterModules(QObject *parent) {
    networkAccessManager = new QNetworkAccessManager(parent);
    executor = new Executor();

    worker = new Worker();

    repository = new ApplicationRepositoryRestClient("http://apps.nxos.org/api");

    registry = new Registry();
    QObject::connect(executor, &Executor::taskCompleted, registry, &Registry::handleTaskCompleted);

    installer = new Deployer();
    installer->setRepository(repository);

    CacheSource *cacheSource = new CacheSource(Cache::getApplicationsCachePath(), parent);

    deployedApplicationsRegistry = new DeployedApplicationsRegistry();
    auto cacheDirLocations = QStandardPaths::standardLocations(QStandardPaths::CacheLocation);
    deployedApplicationsRegistry->setCacheDir(cacheDirLocations.first());
    deployedApplicationsRegistry->setApplicationsDir(QDir::homePath() + "/Applications");

//    updater = new Updater(repository, {cacheSource});
//    updater->setExecutor(executor);

    cache = new Cache;
//    cache->setRepository(repository);
    QObject::connect(registry, &Registry::installedApplicationsChanged, cache,
                     &Cache::handleInstalledApplicationsChanged);

    upgrader = new Upgrader();
//    upgrader->setRepository(repository);
    upgrader->setInstalledApplications(registry->getInstalledApplications());
    QObject::connect(registry, &Registry::installedApplicationsChanged,
                     upgrader, &Upgrader::handleInstalledApplicationsChanged);
}

static QObject *searchControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    SearchController *searchControler = new SearchController(repository);
    return searchControler;
}

static QObject *tasksControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    TasksController *taskControler = new TasksController(executor);
    taskControler->setWorker(worker);
    return taskControler;
}

static QObject *deployControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    DeployController *installControler = new DeployController(installer);
    installControler->setWorker(worker);
    return installControler;
}

static QObject *uninstallControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
//    auto *uninstallController = new RemoveController(repository, registry, executor);
//    return uninstallController;
    return new QObject();
}

static QObject *registryControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    RegistryController *registryControler = new RegistryController(registry);
    return registryControler;
}

static QObject *updaterControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
//    UpdaterController *updaterController = new UpdaterController(updater);
//    return updaterController;
    return new QObject();

}

static QObject *upgraderControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
//    UpgraderController *upgraderController = new UpgraderController(upgrader, repository, registry, executor,
//                                                                    nullptr);
//    return upgraderController;
    return new QObject();
}

static QObject *notificationsControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto *notificationsController = new NotificationsController();
    notificationsController->setExecutor(executor);
    return notificationsController;
}

static QObject *runControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto runController = new RunController(nullptr);
    runController->setDeployedApplicationsRegistry(deployedApplicationsRegistry);
    return runController;
}

static QObject *applicationViewControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto applicationViewController = new ApplicationViewController();
    applicationViewController->setRegistry(registry);
    applicationViewController->setExecutor(executor);
    applicationViewController->setRepository(repository);
    return applicationViewController;
}


static QObject *deployedApplicationsControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto deployedApplicationsController = new DeployedApplicationsController();
    deployedApplicationsController->setWorker(worker);
    deployedApplicationsController->setRegistry(deployedApplicationsRegistry);

    return deployedApplicationsController;
}

void registerQmlModules() {
    qmlRegisterSingletonType<SearchController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                               "SearchController",
                                               searchControllerSingletonProvider);

    qmlRegisterSingletonType<TasksController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                              "TasksController",
                                              tasksControllerSingletonProvider);

    qmlRegisterSingletonType<DeployController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                               "DeployController",
                                               deployControllerSingletonProvider);

    qmlRegisterSingletonType<DeployController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                               "RemoveController",
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

    qmlRegisterSingletonType<NotificationsController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                                      "NotificationsController",
                                                      notificationsControllerSingletonProvider);

    qmlRegisterSingletonType<RunController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                            "RunController", runControllerSingletonProvider);

    qmlRegisterSingletonType<RunController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                            "ApplicationViewController", applicationViewControllerSingletonProvider);

    qmlRegisterSingletonType<DeployedApplicationsController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                            "DeployedApplicationsController", deployedApplicationsControllerSingletonProvider);

}
