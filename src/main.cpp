#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QList>
#include <QIcon>

#include "AppImageHubSource.h"
#include "gateways/SimpleDownloadManager.h"
#include "gateways/CachedDownloadManager.h"
#include "entities/Registry.h"
#include "entities/Executor.h"

#include "ui/SearchControler.h"
#include "ui/TasksController.h"
#include "ui/InstallController.h"
#include "ui/RegistryController.h"

#include <QQmlDebuggingEnabler>

#define QML_MODULE_NAMESPACE "org.nxos.softwarecenter"

Repository *repository;
DownloadManager *downloadManager = nullptr;
QNetworkAccessManager *networkAccessManager = nullptr;
Executor *executor = nullptr;
Registry *registry = nullptr;


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

    networkAccessManager = new QNetworkAccessManager(parent);
    downloadManager = new SimpleDownloadManager(networkAccessManager, parent);


    AppImageHubSource *s = new AppImageHubSource(downloadManager, parent);

    repository = new Repository();
    repository->setSources({s});
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

static QObject *registryControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    RegistryController *registryControler = new RegistryController(registry);
    return registryControler;
}

void registerQmlModules() {
    qmlRegisterSingletonType<SearchControler>(QML_MODULE_NAMESPACE, 1, 0,
                                              "SearchController",
                                              searchControllerSingletonProvider);


    qmlRegisterSingletonType<TasksController>(QML_MODULE_NAMESPACE, 1, 0,
                                       "TasksController",
                                       tasksControllerSingletonProvider);

    qmlRegisterSingletonType<InstallController>(QML_MODULE_NAMESPACE, 1, 0,
                                       "InstallController",
                                       installControllerSingletonProvider);

    qmlRegisterSingletonType<RegistryController>(QML_MODULE_NAMESPACE, 1, 0,
                                       "RegistryController",
                                       registryControllerSingletonProvider);
}
