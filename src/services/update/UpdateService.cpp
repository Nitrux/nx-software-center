#include "UpdateService.h"

#include <QDebug>

UpdateService::UpdateService(QObject *parent)
    : QObject(parent)
    , _updatesAvailable(0)
    , _workerThread(this)
    , _checkUpdatesWorker(nullptr)
    , _busy(false)
{
    qRegisterMetaType<UpdateInformation>();

    _checkUpdatesWorker.moveToThread(&_workerThread);
    _workerThread.start();

    connect(&_checkUpdatesWorker, &UpdatesWorker::taskUpdate, this, &UpdateService::taskUpdate, Qt::QueuedConnection);
    connect(&_checkUpdatesWorker, &UpdatesWorker::updateFound, this, &UpdateService::handleUpdateFound, Qt::QueuedConnection);
    connect(&_checkUpdatesWorker, &UpdatesWorker::updateCompleted, this, &UpdateService::updateDownloaded, Qt::QueuedConnection);
    connect(&_checkUpdatesWorker, &UpdatesWorker::queueCompleted, this, &UpdateService::handleQueueCompleted, Qt::QueuedConnection);
}

void UpdateService::checkUpdate(const QVariant &applicationVariant)
{
    if (applicationVariant.canConvert<ApplicationData>()) {
        auto appData = qvariant_cast<ApplicationData>(applicationVariant);

        _busy = true;
        emit isBusyChanged(_busy);

        _checkUpdatesWorker.queueCheck({appData});
    }
}

void UpdateService::checkUpdates(const ApplicationsList &targetApps)
{
    _busy = true;
    emit isBusyChanged(_busy);

    _checkUpdatesWorker.queueCheck(targetApps);
}

void UpdateService::update(const QVariant &variant)
{
    if (variant.canConvert<ApplicationData>()) {
        auto application = qvariant_cast<ApplicationData>(variant);
        _checkUpdatesWorker.queueUpdate({application});
        return;
    }

    if (variant.canConvert<ApplicationsList>()) {
        auto applications = qvariant_cast<ApplicationsList>(variant);
        _checkUpdatesWorker.queueUpdate(applications);
        return;
    }

    qWarning() << "UpdateService::update: invalid variant value received";
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
void UpdateService::handleUpdateFound(const UpdateInformation &updateInformation)
{
    qDebug() << "Update available " << updateInformation.application.getId();

    const auto &app = updateInformation.application;
    if (updateInformation.updateAvailable) {
        _updatesInformation.insert(app.getId(), updateInformation);

        emit(updatesAvailableCounterChanged(_updatesInformation.count()));
        emit(updateFound(updateInformation));
    }
}
void UpdateService::handleUpdateCompleted(const ApplicationBundle &bundle)
{
    const auto &entries = _updatesInformation.values();
    for (auto &entry : entries) {
        if (entry.application.getId() == bundle.app->getId()) {
            auto newEntry = entry;
            newEntry.checkDate = QDateTime::currentDateTime();
            newEntry.updateAvailable = false;

            _updatesInformation.insert(newEntry.application.getId(), newEntry);
        }
    }
}
bool UpdateService::isBusy()
{
    return _busy;
}
void UpdateService::handleQueueCompleted()
{
    qDebug() << "set busy to false";
    _busy = false;
    emit isBusyChanged(_busy);
}
