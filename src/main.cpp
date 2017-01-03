#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "snapsindex.h"
#include "snapd.h"



int main(int argc, char *argv[])
{

        QGuiApplication app(argc, argv);
        QCoreApplication::addLibraryPath("./");

        QQmlApplicationEngine engine;
        qmlRegisterType<SnapsIndex>("org.nx", 1, 0, "Local");
        // qmlRegisterType<RadioPlayer>(uri, 1, 0, "Store");

        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

        return app.exec();

}


