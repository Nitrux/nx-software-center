#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include <KAuth/KAuthExecuteJob>
#include "localsnaps.h"
#include "snapstore.h"
#include "changes.h"
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

    // HACK TO LOAD THE PLUGIN FROM ITS CUSTOM INSTALL PATH
    // FIXME: well, fix the glib-snapd-qt plugin install path
    engine.addImportPath(QStringLiteral("/usr/lib/qt5/qml/"));
    qDebug() << engine.importPathList();

    qmlRegisterSingletonType<SnapD>("org.nx.softwarecenter", 1, 0, "Snaps", snaps_singletontype_provider);
    qmlRegisterUncreatableType<SnapStore> ("org.nx.softwarecenter", 1, 0, "Store", "Can't be instantiated.");
    qmlRegisterUncreatableType<LocalSnaps> ("org.nx.softwarecenter", 1, 0, "Installed", "Can't be instantiated.");
    qmlRegisterUncreatableType<Changes> ("org.nx.softwarecenter", 1, 0, "Changes", "Can't be instantiated.");
    qmlRegisterUncreatableType<KAuth::ExecuteJob> ("org.nx.softwarecenter", 1, 0, "Job", ".");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}


