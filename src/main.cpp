#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QList>

#include "SearchControler.h"
#include "AppImageHubSource.h"
#include "SimpleDownloadManager.h"
#include "FakeDownloadManager.h"

#define QML_MODULE_NAMESPACE "org.nxos.softwarecenter"

QList<Source*> sources;
DownloadManager *downloadManager = nullptr;
QNetworkAccessManager * networkAccessManager = nullptr;


void initSources(QObject *parent) {
    networkAccessManager = new QNetworkAccessManager(parent);
    downloadManager = new FakeDownloadManager(parent);

    AppImageHubSource *s = new AppImageHubSource(downloadManager, parent);
    sources.append(s);
}

static QObject * searchControllerSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
     Q_UNUSED(engine)
     Q_UNUSED(scriptEngine)

     SearchControler *searchControler = new SearchControler();
     searchControler->setSources(sources);
     return searchControler;
 }

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    initSources(nullptr);
    qmlRegisterSingletonType<SearchControler>(QML_MODULE_NAMESPACE, 1, 0,
                                              "SearchController",
                                              searchControllerSingletonProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
