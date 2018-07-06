#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QIcon>

#include <ui/UpgraderController.h>
#include <ui/NotificationsController.h>

#include "gateways/CacheSource.h"
#include "entities/Registry.h"
#include "entities/Executor.h"
#include "entities/Updater.h"
#include "entities/Cache.h"

#include "ui/SearchController.h"
#include "ui/TasksController.h"
#include "ui/InstallController.h"
#include "ui/UninstallController.h"
#include "ui/RegistryController.h"
#include "ui/UpdaterController.h"
#include "ui/RunController.h"
#include "ui/ApplicationViewController.h"

#define QML_MODULE_NAMESPACE "org.nxos.softwarecenter"
#define QML_MODULE_MAJOR_VERSION 1

Repository* repository;
QNetworkAccessManager* networkAccessManager = nullptr;
Executor* executor = nullptr;
RestClient* restClient = nullptr;
Installer* installer = nullptr;
Registry* registry = nullptr;
Updater* updater = nullptr;
Cache* cache = nullptr;
Upgrader* upgrader = nullptr;

void registerQmlModules();
void initSoftwareCenterModules(QObject* parent);

Q_DECLARE_METATYPE(Application)
Q_DECLARE_METATYPE(QList<Application>)
Q_DECLARE_METATYPE(UpgradeList)
void registerMetatypes();

int main(int argc, char* argv[])
{
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

void registerMetatypes()
{
    qRegisterMetaType<Application>("Application");
    qRegisterMetaType<QList<Application>>("ApplicationList");
    qRegisterMetaType<UpgradeList>("UpgradeList");
}

void initSoftwareCenterModules(QObject* parent)
{
    networkAccessManager = new QNetworkAccessManager(parent);
    executor = new Executor();

    restClient = new RestClient("http://apps.nxos.org/api");
    restClient->setNetworkAccessManager(networkAccessManager);

    registry = new Registry();
    QObject::connect(executor, &Executor::taskCompleted, registry, &Registry::handleTaskCompleted);

    installer = new Installer();
    installer->setRestClient(restClient);
    repository = new Repository();

    CacheSource* cacheSource = new CacheSource(Cache::getApplicationsCachePath(), parent);

    updater = new Updater(repository, {cacheSource});
    updater->setExecutor(executor);

    cache = new Cache;
    cache->setRepository(repository);
    QObject::connect(registry, &Registry::installedApplicationsChanged, cache,
            &Cache::handleInstalledApplicationsChanged);

    upgrader = new Upgrader();
    upgrader->setRepository(repository);
    upgrader->setInstalledApplications(registry->getInstalledApplications());
    QObject::connect(registry, &Registry::installedApplicationsChanged,
            upgrader, &Upgrader::handleInstalledApplicationsChanged);
}

static QObject* searchControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    SearchController* searchControler = new SearchController(restClient);
    return searchControler;
}

static QObject* tasksControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    TasksController* taskControler = new TasksController(executor);
    return taskControler;
}

static QObject* installControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    InstallController* installControler = new InstallController(installer);
    return installControler;
}

static QObject* uninstallControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    auto* uninstallController = new UninstallController(repository, registry, executor);
    return uninstallController;
}

static QObject* registryControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    RegistryController* registryControler = new RegistryController(registry);
    return registryControler;
}

static QObject* updaterControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    UpdaterController* updaterController = new UpdaterController(updater);
    return updaterController;
}

static QObject* upgraderControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    UpgraderController* upgraderController = new UpgraderController(upgrader, repository, registry, executor,
            nullptr);
    return upgraderController;
}

static QObject* notificationsControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    auto* notificationsController = new NotificationsController();
    notificationsController->setExecutor(executor);
    return notificationsController;
}

static QObject* runControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    auto runController = new RunController(registry);
    return runController;
}

static QObject* applicationViewControllerSingletonProvider(QQmlEngine*, QJSEngine*)
{
    auto applicationViewController = new ApplicationViewController();
    applicationViewController->setRegistry(registry);
    applicationViewController->setExecutor(executor);
    applicationViewController->setExplorer(restClient);
    return applicationViewController;
}

void registerQmlModules()
{
    qmlRegisterSingletonType<SearchController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
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

    qmlRegisterSingletonType<NotificationsController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
            "NotificationsController",
            notificationsControllerSingletonProvider);

    qmlRegisterSingletonType<RunController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
            "RunController", runControllerSingletonProvider);

    qmlRegisterSingletonType<RunController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
            "ApplicationViewController", applicationViewControllerSingletonProvider);
}
