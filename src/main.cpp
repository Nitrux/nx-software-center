#include <QApplication>
#include <QCommandLineParser>

#include <QQmlContext>

#include "NXSCApp.h"
#include "ResponseDTO/application.h"
#include "ResponseDTO/category.h"
#include "models/progressmanager.h"
#include "models/storemodel.h"
#include "utils/nx.h"

#include <MauiKit/FileBrowsing/fmstatic.h>

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
