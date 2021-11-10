#include <QApplication>
#include <QCommandLineParser>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>

#include "utils/nx.h"
#include "models/appsmodel.h"
#include "models/progressmanager.h"
#include "models/app.h"
#include "models/categoriesmodel.h"
#include "models/storemodel.h"
#include "ResponseDTO/application.h"
#include "ResponseDTO/category.h"


#include <MauiKit/Core/mauiapp.h>
#include <MauiKit/FileBrowsing/fmstatic.h>

#include <KI18n/KLocalizedString>
#include <services/TaskManager.h>

#include "../nx_sc_version.h"

#define NX_URI "org.nx.softwarecenter"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

	QApplication app(argc, argv);

    app.setOrganizationName(QStringLiteral("Nitrux"));
	app.setWindowIcon(QIcon(":/nx-software-center.svg"));

    MauiApp::instance()->setIconName("qrc:/nx-software-center.svg");

	KLocalizedString::setApplicationDomain("nx-software-center");
    KAboutData about(QStringLiteral("nx-software-center"), i18n("NX Software Center"), NX_SC_VERSION_STRING, i18n("Browse and install AppImages."),
                     KAboutLicense::LGPL_V3, i18n("© 2019-%1 Nitrux Development Team", QString::number(QDate::currentDate().year())), QString(GIT_BRANCH) + "/" + QString(GIT_COMMIT_HASH));
	about.addAuthor(i18n("Camilo Higuita"), i18n("Developer"), QStringLiteral("milo.h@aol.com"));
    about.addAuthor(i18n("Anupam Basak"), i18n("Developer"), QStringLiteral("anupam.basak27@gmail.com"));
	about.setHomepage("https://nxos.org");
    about.setProductName("nitrux/nx-software-center");
	about.setBugAddress("https://github.com/nitrux/issues");
	about.setOrganizationDomain(NX_URI);
	about.setProgramLogo(app.windowIcon());

	KAboutData::setApplicationData(about);

	QCommandLineParser parser;
	parser.process(app);

	about.setupCommandLine(&parser);
	about.processCommandLine(&parser);

	FMStatic::createDir(NX::AppsPath, QString());

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl)
	{
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);

	}, Qt::QueuedConnection);

	qmlRegisterType<App>("NXModels", 1, 0, "App");
	qmlRegisterType<AppsModel>("NXModels", 1, 0, "Apps");
	qmlRegisterType<ProgressManager>("NXModels", 1, 0, "ProgressManager");
    qmlRegisterAnonymousType<Package>("NXModels", 1);
    qmlRegisterAnonymousType<Application>("NXModels", 1);
    qmlRegisterType<Category>("NXModels", 1, 0, "Category");
	qmlRegisterType<StoreModel>("NXModels", 1, 0, "Store");
	qmlRegisterType<CategoriesModel>("NXModels", 1, 0, "Categories");

	auto taskManager = new TaskManager(&app);
    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty("taskManagerCtx", taskManager);
    qmlRegisterUncreatableType<Task>("NXModels", 1, 0, "Task", "Tasks can only be created from the Task Manager");
    
	engine.load(url);

    return app.exec();
}
