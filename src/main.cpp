#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocalSocket>
#include <QQmlEngine>
#include <QQmlNetworkAccessManagerFactory>
#include <QtQml>

#include <KAuth/KAuthExecuteJob>

#include "entities/simplefileregistry.h"

#include "gateways/appimagehubrepository.h"
#include "gateways/kf5downloadmanager.h"

#include "ui/registrycontroller.h"
#include "ui/searchviewcontroller.h"
#include "ui/taskcontroller.h"
#include "ui/taskscontroller.h"

Registry *registry = nullptr;
Repository *repository = nullptr;
KF5DownloadManager *downloadManager = nullptr;

SearchViewController *searchviewcontroller = nullptr;
TasksController *tasksController = nullptr;
RegistryController *registryController = nullptr;

static QObject *searchviewcontroller_singletontype_provider(QQmlEngine *engine,
                                                            QJSEngine *) {
  if (searchviewcontroller == nullptr) {
    Q_ASSERT(repository != nullptr);
    QList<Repository *> repositoryList(QList<Repository *>{repository});
    searchviewcontroller =
        new SearchViewController(registry, repositoryList, engine);
  }

  return dynamic_cast<QObject *>(searchviewcontroller);
}

static QObject *taskscontroller_singletontype_provider(QQmlEngine *engine,
                                                       QJSEngine *) {
  if (tasksController == nullptr) {
    Q_ASSERT(repository != nullptr);
    QList<Repository *> repositoryList(QList<Repository *>{repository});
    tasksController =
        new TasksController(repositoryList, registry, downloadManager, engine);
  }

  return dynamic_cast<QObject *>(tasksController);
}

static QObject *registrycontroller_singletontype_provider(QQmlEngine *engine,
                                                          QJSEngine *) {
  if (registryController == nullptr) {
    Q_ASSERT(registry != nullptr);
    registryController = new RegistryController(registry, engine);
  }

  return dynamic_cast<QObject *>(registryController);
}

int main(int argc, char *argv[]) {
  const char *uri = "org.nx.softwarecenter";

  QGuiApplication app(argc, argv);
  QCoreApplication::addLibraryPath("./");

  QCoreApplication::setOrganizationName("NXOS");
  QCoreApplication::setOrganizationDomain("nxos.org");
  QCoreApplication::setApplicationName("nx-software-center");

  app.setWindowIcon(QIcon::fromTheme("nx-software-center"));
  QQmlApplicationEngine engine;

  // Init view controllers
  downloadManager = new KF5DownloadManager();
  registry = new SimpleFileRegistry();
  repository = new Repository();

  qRegisterMetaType<TaskController::TaskState>("TaskState");
  qmlRegisterUncreatableType<TaskController>(
      uri, 1, 0, "Task", "Task can only be created by the TasksController");
  qmlRegisterSingletonType<SearchViewController>(
      uri, 1, 0, "SearchViewController",
      searchviewcontroller_singletontype_provider);
  qmlRegisterSingletonType<TasksController>(
      uri, 1, 0, "TasksController", taskscontroller_singletontype_provider);
  qmlRegisterSingletonType<RegistryController>(
      uri, 1, 0, "RegistryController",
      registrycontroller_singletontype_provider);

  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  return app.exec();
}
