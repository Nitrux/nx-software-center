#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QIcon>

#include "AppImageHubSource.h"
#include "gateways/SimpleDownloadManager.h"
#include "gateways/CachedDownloadManager.h"
#include "entities/Registry.h"
#include "entities/Executor.h"
#include "entities/Updater.h"

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


void registerQmlModules();
void initSoftwareCenterModules(QObject *parent);

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlDebuggingEnabler enabler;
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
    AppImageHubSource *s = new AppImageHubSource(downloadManager, parent);
    updater = new Updater(repository, {s});
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
}
