#include <QCommandLineParser>

#include "NXSCApp.h"

int main(int argc, char *argv[])
{
    // Attribute Qt::AA_EnableHighDpiScaling must be set before QCoreApplication is created
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QScopedPointer<NXSCApp> app(new NXSCApp(argc, argv));
    app->parseCommands();
    app->setupQMLEngine();
    app->setupApplicationsRegistry();

    FMStatic::createDir(NX::AppsPath, QString());

    return app->exec();
}
