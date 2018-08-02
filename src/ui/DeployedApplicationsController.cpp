//
// Created by alexis on 7/27/18.
//

#include <tasks/TaskMetadata.h>
#include "DeployedApplicationsController.h"
#include "entities/LocalizationUtils.h"

DeployedApplicationsController::DeployedApplicationsController(QObject *parent):
    QObject(parent), worker(nullptr)
{
}

void DeployedApplicationsController::setWorker(Worker *worker)
{
    if (worker)
        disconnect(worker, nullptr, this, nullptr);

    DeployedApplicationsController::worker = worker;
    connect(worker, &Worker::taskCompleted, this, &DeployedApplicationsController::handleWorkerTaskCompleted);
}

void DeployedApplicationsController::updateApplicationsList()
{
    auto apps = registry->listApplications();

    QVariantList newList;
    QStringList newListIds;
    for (const auto &app: apps) {
        QVariantMap v = app.toVariant();
        v["name"] = LocalizationUtils::getLocalizedValue(app.name);
        v["abstract"] = LocalizationUtils::getLocalizedValue(app.abstract);
        v["fileSize"] = formatMemoryValue(app.fileSize);
        newList << v;
        newListIds << app.id;
    }


    applicationList = newList;
    applicationIds = newListIds;

    emit applicationListChanged();
}

QString DeployedApplicationsController::formatMemoryValue(float num) {
    QStringList list;
    list << "KiB" << "MiB" << "GiB" << "TiB";

    QStringListIterator i(list);
    QString unit("bytes");

    while (num >= 1024.0 && i.hasNext()) {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num, 'f', 2) + " " + unit;
}

void DeployedApplicationsController::handleWorkerTaskCompleted(const QVariantMap task)
{
    if (task[TaskMetadata::KEY_TYPE] == TaskMetadata::VALUE_TYPE_DELPOY ||
            task[TaskMetadata::KEY_TYPE] == TaskMetadata::VALUE_TYPE_REMOVE) {

        updateApplicationsList();
    }
}

void DeployedApplicationsController::setRegistry(DeployedApplicationsRegistry *registry) {
    DeployedApplicationsController::registry = registry;

    updateApplicationsList();
}
