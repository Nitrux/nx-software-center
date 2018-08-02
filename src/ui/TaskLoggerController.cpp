#include "TaskLoggerController.h"

TaskLoggerController::TaskLoggerController(TaskLogger *registry, QObject *parent) :
    QObject(parent), registry(registry), model(new RegistryListModel(this))
{
    model->setRecords(registry->getRecords());

    connect(registry, &TaskLogger::recordsChanged,
            this, &TaskLoggerController::handleRecordsChanged);
}

void TaskLoggerController::handleRecordsChanged(const QList<QVariantMap> &records)
{
    model->setRecords(records);
}

void TaskLoggerController::clearRecords() {
    registry->clearRecords();
}
