#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include <KAuth/KAuthExecuteJob>

#include <Snapd/App>
#include "snapdclientkauthwrapper.h"



static QObject *snapdkauthwrapper_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
 {
     Q_UNUSED(engine)
     Q_UNUSED(scriptEngine)

     return new SnapdClientKAuthWrapper();
 }

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");

    QQmlApplicationEngine engine;

    // HACK TO LOAD THE PLUGIN FROM ITS CUSTOM INSTALL PATH
    // FIXME: well, fix the glib-snapd-qt plugin install path
    engine.addImportPath(QStringLiteral("/usr/lib/qt5/qml/"));
    qDebug() << engine.importPathList();

    qmlRegisterSingletonType<SnapdClientKAuthWrapper>("org.nx.softwarecenter", 1, 0, "SnapdRootClient", snapdkauthwrapper_singletontype_provider);
    qmlRegisterUncreatableType<KAuth::ExecuteJob> ("org.nx.softwarecenter", 1, 0, "Job", ".");
    qmlRegisterUncreatableType<QSnapdApp> ("org.nx.softwarecenter", 1, 0, "Job", ".");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}


