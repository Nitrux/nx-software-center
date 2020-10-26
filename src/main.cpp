#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QCommandLineParser>
#include <QDebug>

#include "utils/nx.h"
#include "models/appsmodel.h"
#include "models/progressmanager.h"
#include "models/app.h"
#include "models/categoriesmodel.h"
#include "models/storemodel.h"
#include "ResponseDTO/application.h"
#include "ResponseDTO/category.h"

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
#include "mauiapp.h"
#include "fmstatic.h"
#else
#include <MauiKit/mauiapp.h>
#include <MauiKit/fmstatic.h>
#endif

#if defined MAUIKIT_STYLE
#include <MauiKit/mauikit.h>
#endif

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef Q_OS_ANDROID
	QGuiApplication app(argc, argv);
#else
	QApplication app(argc, argv);
#endif

#ifdef MAUIKIT_STYLE
	MauiKit::getInstance().initResources();
#endif

	app.setApplicationName(NX::appName);
	app.setApplicationVersion(NX::version);
	app.setApplicationDisplayName(NX::displayName);
	app.setOrganizationName(NX::orgName);
	app.setOrganizationDomain(NX::orgDomain);
	app.setWindowIcon(QIcon(":/nx-software-center.svg"));
	MauiApp::instance()->setHandleAccounts(false); //for now index can not handle cloud accounts
//	MauiApp::instance()->setCredits ({QVariantMap({{"name", "Camilo Higuita"}, {"email", "milo.h@aol.com"}, {"year", "2019-2020"}}),
//									 QVariantMap({{"name", "Anupam Basak"}, {"email", "anupam.basak27@gmail.com"}, {"year", "2019-2020"}})});

	QCommandLineParser parser;
	parser.setApplicationDescription(NX::description);
	const QCommandLineOption versionOption = parser.addVersionOption();
	parser.addOption(versionOption);
	parser.process(app);

#ifdef STATIC_KIRIGAMI
	KirigamiPlugin::getInstance().registerTypes();
#endif

#ifdef STATIC_MAUIKIT
	MauiKit::getInstance().registerTypes();
#endif

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
	qmlRegisterType<Package>("NXModels", 1, 0, "Package");
	qmlRegisterType<Application>();
	qmlRegisterType<Category>();
	qmlRegisterType<StoreModel>("NXModels", 1, 0, "Store");
	qmlRegisterType<CategoriesModel>("NXModels", 1, 0, "Categories");
	engine.load(url);
	return app.exec();
}
