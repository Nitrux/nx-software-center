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

#if defined Q_OS_MACOS || defined Q_OS_WIN
#include <KF5/KI18n/KLocalizedString>
#else
#include <KI18n/KLocalizedString>
#endif

#if defined MAUIKIT_STYLE
#include <MauiKit/mauikit.h>
#endif

#define NX_URI "org.maui.nx"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
	QCoreApplication::setAttribute(Qt::AA_DisableSessionManager, true);

#ifdef Q_OS_ANDROID
	QGuiApplication app(argc, argv);
	if (!MAUIAndroid::checkRunTimePermissions({"android.permission.WRITE_EXTERNAL_STORAGE"}))
		return -1;
#else
	QApplication app(argc, argv);
#endif

	app.setOrganizationName(QStringLiteral("Maui"));
	app.setWindowIcon(QIcon(":/nx-software-center.svg"));
	MauiApp::instance()->setHandleAccounts(false); //for now index can not handle cloud accounts
	MauiApp::instance()->setIconName("qrc:/nx-software-center.svg");

	KLocalizedString::setApplicationDomain("nx-software-center");
	KAboutData about(QStringLiteral("nx-software-center"), i18n("NX Software Center"), NX::version, i18n("NX Software Center distributes AppImages for GNU Linux and APKS for Android."),
					 KAboutLicense::LGPL_V3, i18n("© 2019-2020 Nitrux Development Team"));
	about.addAuthor(i18n("Camilo Higuita"), i18n("Developer"), QStringLiteral("milo.h@aol.com"));
	about.setHomepage("https://nxos.org");
	about.setProductName("maui/nx-software-center");
	about.setBugAddress("https://github.com/nitrux/issues");
	about.setOrganizationDomain(NX_URI);
	about.setProgramLogo(app.windowIcon());

	KAboutData::setApplicationData(about);

	QCommandLineParser parser;
	parser.process(app);

	about.setupCommandLine(&parser);
	about.processCommandLine(&parser);


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
