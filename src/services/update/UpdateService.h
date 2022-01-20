#pragma once
// system

// libraries
#include <QList>
#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QThread>

// local
#include "UpdateInformation.h"
#include "UpdatesWorker.h"
#include "services/ProgressNotification.h"

class UpdateService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int updatesAvailableCount READ getUpdatesAvailableCounter NOTIFY updatesAvailableCounterChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY isBusyChanged)

public:
    explicit UpdateService(QObject *parent = nullptr);
    ~UpdateService() override;

    Q_SCRIPTABLE [[nodiscard]] int getUpdatesAvailableCounter() const;
    Q_SIGNAL void updatesAvailableCounterChanged(int updatesAvailable);

    Q_SCRIPTABLE void checkUpdate(const QVariant &applicationVariant);
    Q_SCRIPTABLE void checkUpdates(const ApplicationsList &targetApps);

    Q_SCRIPTABLE void update(const QVariant &variant);

    // notifies when a new bundle is available
    Q_SIGNAL void updateDownloaded(const ApplicationBundle &bundle);

    // use it to forward tasks updates
    Q_SIGNAL void progressNotification(const ProgressNotification &update);

    // notifies when the performing some back ground task
    bool isBusy();
    Q_SIGNAL void isBusyChanged(bool isBusy);

    Q_SIGNAL void updateFound(const UpdateInformation &updateInformation);
    Q_SIGNAL void updateCompleted(QString appId);

private:
    Q_SLOT void handleUpdateFound(const UpdateInformation &updateInformation);
    Q_SLOT void handleUpdateCompleted(const ApplicationBundle &bundle);
    Q_SLOT void handleQueueCompleted();

    int _updatesAvailable;

    QMap<QString, UpdateInformation> _updatesInformation; // app id - UpdateInformation
    QThread _workerThread;
    UpdatesWorker _checkUpdatesWorker;
    bool _busy;
};
