#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QCommandLineParser>
#include <QDebug>
#include "utils/nx.h"

#ifdef Q_OS_ANDROID
#include <QGuiApplication>
#include <QIcon>
#else
#include <QApplication>
#endif

#ifdef STATIC_KIRIGAMI
#include "3rdparty/kirigami/src/kirigamiplugin.h"
#endif

#ifdef STATIC_MAUIKIT
#include "3rdparty/mauikit/src/mauikit.h"
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef Q_OS_ANDROID
    QGuiApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif

    app.setApplicationName(NX::app);
    app.setApplicationVersion(NX::version);
    app.setApplicationDisplayName(NX::app);
    app.setOrganizationName(NX::org);
    app.setWindowIcon(QIcon(":/nx-software-center.svg"));

    QCommandLineParser parser;
    parser.setApplicationDescription(NX::description);
    const QCommandLineOption versionOption = parser.addVersionOption();
    parser.addOption(versionOption);
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    QStringList paths;

    if(!args.isEmpty())
        paths = args;

#ifdef STATIC_KIRIGAMI
    KirigamiPlugin::getInstance().registerTypes();
#endif

#ifdef STATIC_MAUIKIT
    MauiKit::getInstance().registerTypes();
#endif

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url, paths](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);

    }, Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
