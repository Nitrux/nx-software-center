#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "localsnaps.h"
#include "snapstore.h"
#include "snapd.h"



static QObject *snaps_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
 {
     Q_UNUSED(engine)
     Q_UNUSED(scriptEngine)

     return SnapD::instance();
 }

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<SnapD>("org.nx.softwarecenter", 1, 0, "Snaps", snaps_singletontype_provider);
    qmlRegisterType<SnapStore>("org.nx.softwarecenter", 1, 0, "StoreIndex");
    qmlRegisterType<LocalSnaps>("org.nx.softwarecenter", 1, 0, "LocalIndex");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}


