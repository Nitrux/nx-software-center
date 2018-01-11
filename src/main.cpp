#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QList>

#include "AppImageHubSource.h"
#include "gateways/SimpleDownloadManager.h"
#include "gateways/CachedDownloadManager.h"
#include "entities/Registry.h"
#include "entities/Executor.h"

#include "ui/SearchControler.h"
#include "ui/TasksController.h"
#include "ui/InstallController.h"


#define QML_MODULE_NAMESPACE "org.nxos.softwarecenter"

Repository *repository;
DownloadManager *downloadManager = nullptr;
QNetworkAccessManager *networkAccessManager = nullptr;
Executor *executor = nullptr;


void registerQmlModules();
void initSoftwareCenterModules(QObject *parent);

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    initSoftwareCenterModules(nullptr);
    registerQmlModules();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

void initSoftwareCenterModules(QObject *parent) {
    executor = new Executor();

    networkAccessManager = new QNetworkAccessManager(parent);
    downloadManager = new CachedDownloadManager(networkAccessManager, parent);


    AppImageHubSource *s = new AppImageHubSource(downloadManager, parent);

    repository = new Repository();
    repository->setSources({s});
}



static QObject *searchControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    SearchControler *searchControler = new SearchControler(repository);
    return searchControler;
}

static QObject *registrySingletonProvider(QQmlEngine *, QJSEngine *) {
    Registry *r = new Registry(nullptr);
    return r;
}

static QObject *tasksControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    TasksController *taskControler = new TasksController(executor);
    return taskControler;
}

static QObject *installControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    InstallController *installControler = new InstallController(repository, executor, downloadManager);
    return installControler;
}

void registerQmlModules() {
    qmlRegisterSingletonType<SearchControler>(QML_MODULE_NAMESPACE, 1, 0,
                                              "SearchController",
                                              searchControllerSingletonProvider);

    qmlRegisterSingletonType<Registry>(QML_MODULE_NAMESPACE, 1, 0,
                                       "Registry",
                                       registrySingletonProvider);

    qmlRegisterSingletonType<Registry>(QML_MODULE_NAMESPACE, 1, 0,
                                       "TasksController",
                                       tasksControllerSingletonProvider);

    qmlRegisterSingletonType<Registry>(QML_MODULE_NAMESPACE, 1, 0,
                                       "InstallController",
                                       installControllerSingletonProvider);
}
