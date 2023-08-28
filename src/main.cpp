// libraries
#include <MauiKit3/FileBrowsing/fmstatic.h>
#include <QCommandLineParser>

// local
#include "NXSCApp.h"
#include "nx.h"

int main(int argc, char *argv[])
{
    // Attribute Qt::AA_EnableHighDpiScaling must be set before QCoreApplication is created
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QScopedPointer<NXSCApp> app(new NXSCApp(argc, argv));
    app->parseCommands();
    app->setup();

    FMStatic::createDir(NX::AppsPath, QString());

    return app->exec();
}
