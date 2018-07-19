#include <QList>
#include "InstallController.h"

InstallController::InstallController(Installer *installer, QObject *parent)
        : QObject(parent), installer(installer) {}

void InstallController::install(const QString &application_id) {
    auto task = installer->buildInstallLatestReleaseTask(application_id);
    qInfo() << Task::toVariant(task);
    task->setDeleteOnceCompleted(true);
    if (worker)
        worker->execute(task);

}

void InstallController::setWorker(Worker *worker) {
    InstallController::worker = worker;
}
