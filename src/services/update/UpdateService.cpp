#include "UpdateService.h"

#include "services/CheckUpdateBulkTask.h"

UpdateService::UpdateService(QObject *parent)
    : QObject(parent)
    , _updatesAvailable(0)
    , _workerThread(this)
    , _checkUpdatesWorker(this)
{
    qRegisterMetaType<UpdateCheckData>();

    _checkUpdatesWorker.moveToThread(&_workerThread);
    _workerThread.start();

    connect(&_checkUpdatesWorker, &UpdatesWorker::taskUpdate, this, &UpdateService::taskUpdate, Qt::QueuedConnection);
    connect(&_checkUpdatesWorker, &UpdatesWorker::updateInformation, this, &UpdateService::handleUpdateInformation, Qt::QueuedConnection);
    connect(&_checkUpdatesWorker, &UpdatesWorker::updateCompleted, this, &UpdateService::updateDownloaded, Qt::QueuedConnection);
}

void UpdateService::checkUpdates(const QList<ApplicationData> &applications)
{
    _checkUpdatesWorker.queueCheck(applications);
}

void UpdateService::update(const QList<ApplicationData> &applications)
{
    _checkUpdatesWorker.queueUpdate(applications);
}

int UpdateService::getUpdatesAvailable() const
{
    return _updatesAvailable;
}
UpdateService::~UpdateService()
{
    _workerThread.exit();
    _workerThread.wait(10);
}

void UpdateService::handleUpdateInformation(const UpdateCheckData &updateInformation)
{
    const auto &app = updateInformation.application;
    _updatesInformation.insert(app.getId(), updateInformation);

    refreshUpdatesAvailable();
}
void UpdateService::handleUpdateCompleted(const ApplicationBundle &bundle)
{
    const auto &entries = _updatesInformation.values();
    for (auto &entry : entries) {
        if (entry.application.getId() == bundle.app->getId()) {
            auto newEntry = entry;
            newEntry.lastCheckDate = QDateTime::currentDateTime();
            newEntry.updateAvailable = false;

            _updatesInformation.insert(newEntry.application.getId(), newEntry);
        }
    }
}
void UpdateService::refreshUpdatesAvailable()
{
    _updatesAvailable = 0;
    const auto &entries = _updatesInformation.values();
    for (const auto &entry : entries) {
        if (entry.updateAvailable)
            _updatesAvailable++;
    }

    emit(updatesAvailableChanged(_updatesAvailable));
}
