#include "UpdateService.h"

#include <QDebug>
#include <QMetaObject>

UpdateService::UpdateService(QObject *parent)
    : QObject(parent)
    , _workerThread(this)
    , _checkTaskWorker()
    , _updateTaskWorker()
    , _isCheckTaskRunning(false)
    , _isUpdateTaskRunning(false)
{
    qRegisterMetaType<ApplicationUpdateData>();

    // move workers to another thread to avoid blocking the main thread
    _checkTaskWorker.moveToThread(&_workerThread);
    _updateTaskWorker.moveToThread(&_workerThread);
    _workerThread.start();

    connect(&_checkTaskWorker, &UpdateCheckWorker::updateFound, this, &UpdateService::handleUpdateInformation, Qt::QueuedConnection);
    connect(&_checkTaskWorker, &UpdateCheckWorker::checkCompleted, this, &UpdateService::handleCheckCompleted, Qt::QueuedConnection);

    connect(&_updateTaskWorker, &UpdateWorker::queueCompleted, this, &UpdateService::handleUpdateQueueCompleted);
    connect(&_updateTaskWorker, &UpdateWorker::updateCompleted, this, &UpdateService::handleUpdateInformation);

    // forward task progress notifications
    connect(&_checkTaskWorker, &UpdateCheckWorker::progressNotification, this, &UpdateService::progressNotification, Qt::QueuedConnection);
    connect(&_updateTaskWorker, &UpdateWorker::progressNotification, this, &UpdateService::progressNotification, Qt::QueuedConnection);
}

void UpdateService::checkUpdates(const ApplicationsList &targetApps)
{
    _isCheckTaskRunning = true;
    emit(checkTaskRunningChanged(_isCheckTaskRunning));
    qDebug() << "Starting update lockup for" << targetApps.length() << " applications";

    // invoke using Qt::QueuedConnection to ensure it's executed in the worker thread

    QMetaObject::invokeMethod(&_checkTaskWorker, "checkUpdates", Qt::QueuedConnection, Q_ARG(ApplicationsList, targetApps));
}

int UpdateService::getUpdatesAvailableCounter() const
{
    return _updatesInformation.size();
}

UpdateService::~UpdateService()
{
    _workerThread.exit();
    _workerThread.wait(10);
}

void UpdateService::handleUpdateInformation(const ApplicationUpdateData &updateInformation)
{
    const auto &app = updateInformation.application;
    if (updateInformation.updateAvailable) {
        _updatesInformation.insert(app.getId(), updateInformation);
        emit(updatesAvailableCounterChanged(_updatesInformation.count()));
    } else {
        if (_updatesInformation.contains(app.getId())) {
            _updatesInformation.remove(app.getId());
            emit(updatesAvailableCounterChanged(_updatesInformation.count()));
        }
    }

    emit(applicationUpdateDataChanged(updateInformation));
}

bool UpdateService::isCheckTaskRunning() const
{
    return _isCheckTaskRunning;
}
void UpdateService::handleCheckCompleted()
{
    _isCheckTaskRunning = false;
    emit(checkTaskRunningChanged(_isCheckTaskRunning));
}

void UpdateService::update(const QString &appId)
{
    if (_updatesInformation.contains(appId)) {
        const auto targetUpdateInformation = _updatesInformation.value(appId);
        update(targetUpdateInformation.application);
    } else {
        qDebug() << "UpdateService::update"
                 << "no update information for" << appId;
    }
}
bool UpdateService::isUpdateTaskRunning() const
{
    return _isUpdateTaskRunning;
}
void UpdateService::handleUpdateQueueCompleted()
{
    _isUpdateTaskRunning = false;
    emit(updateTaskRunningChanged(_isCheckTaskRunning));
}

void UpdateService::update(const Application &application)
{
    // use invoke method to ensure that the method is executed in the target thread
    QMetaObject::invokeMethod(&_updateTaskWorker, "enqueue", Qt::QueuedConnection, Q_ARG(Application, application));

    _isUpdateTaskRunning = true;
    emit(updateTaskRunningChanged(_isUpdateTaskRunning));
}
void UpdateService::update(const ApplicationsList &targetApps)
{
    for (const auto &app : targetApps)
        update(app);
}
