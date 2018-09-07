#include <QList>
#include "DeployController.h"

DeployController::DeployController(Deployer *installer, QObject *parent)
        : QObject(parent), installer(installer) {}

void DeployController::deploy(const QString &application_id) {
    auto task = installer->buildDeployLatestReleaseTask(application_id);
    task->setDeleteOnceCompleted(true);
    if (worker)
        worker->execute(task);
}

void DeployController::setWorker(Worker *worker) {
    DeployController::worker = worker;
}
