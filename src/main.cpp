#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QList>

#include "ui/SearchControler.h"
#include "AppImageHubSource.h"
#include "gateways/SimpleDownloadManager.h"
#include "gateways/CachedDownloadManager.h"
#include "entities/Registry.h"

#define QML_MODULE_NAMESPACE "org.nxos.softwarecenter"

QList<Source *> sources;
DownloadManager *downloadManager = nullptr;
QNetworkAccessManager *networkAccessManager = nullptr;


void initSources(QObject *parent) {
    networkAccessManager = new QNetworkAccessManager(parent);
    downloadManager = new CachedDownloadManager(parent);

    AppImageHubSource *s = new AppImageHubSource(downloadManager, parent);
    sources.append(s);
}

static QObject *searchControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    SearchControler *searchControler = new SearchControler();
    searchControler->setSources(sources);
    return searchControler;
}

static QObject *registrySingletonProvider(QQmlEngine *, QJSEngine *) {
    Registry *r = new Registry(nullptr);
    return r;
}
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    initSources(nullptr);
    qmlRegisterSingletonType<SearchControler>(QML_MODULE_NAMESPACE, 1, 0,
                                              "SearchController",
                                              searchControllerSingletonProvider);

    qmlRegisterSingletonType<Registry>(QML_MODULE_NAMESPACE, 1, 0,
                                              "Registry",
                                              registrySingletonProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
