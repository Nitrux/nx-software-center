#pragma once
// system

// libraries
#include <QList>
#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QThread>

// local
#include "ApplicationUpdateData.h"
#include "UpdateCheckWorker.h"
#include "UpdateWorker.h"
#include "services/TaskData.h"

class UpdateService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int updatesAvailableCount READ getUpdatesAvailableCounter NOTIFY updatesAvailableCounterChanged)
    Q_PROPERTY(bool checkTaskRunning READ isCheckTaskRunning NOTIFY checkTaskRunningChanged)
    Q_PROPERTY(bool updateTaskRunning READ isUpdateTaskRunning NOTIFY updateTaskRunningChanged)

public:
    explicit UpdateService(QObject *parent = nullptr);
    ~UpdateService() override;

    Q_SCRIPTABLE [[nodiscard]] int getUpdatesAvailableCounter() const;
    Q_SIGNAL void updatesAvailableCounterChanged(int updatesAvailable);

    Q_SCRIPTABLE void update(const QString &appId);
    Q_SCRIPTABLE void update(const ApplicationData &application);
    Q_SCRIPTABLE void update(const ApplicationsList &targetApps);

    Q_SCRIPTABLE void checkUpdates(const ApplicationsList &targetApps);

    // use it to forward tasks updates
    Q_SIGNAL void progressNotification(const TaskData &update);

    // notifies if the check for updates task is running
    [[nodiscard]] bool isCheckTaskRunning() const;
    Q_SIGNAL void checkTaskRunningChanged(bool isRunning);

    [[nodiscard]] bool isUpdateTaskRunning() const;
    Q_SIGNAL void updateTaskRunningChanged(bool isRunning);

    // notifies when there is a change in the update information of a given application
    Q_SIGNAL void applicationUpdateDataChanged(const ApplicationUpdateData &updateInformation);

private:
    Q_SLOT void handleUpdateInformation(const ApplicationUpdateData &updateInformation);
    Q_SLOT void handleCheckCompleted();
    Q_SLOT void handleUpdateQueueCompleted();

    QMap<QString, ApplicationUpdateData> _updatesInformation; // app id - UpdateInformation
    QThread _workerThread;
    UpdateCheckWorker _checkTaskWorker;
    UpdateWorker _updateTaskWorker;

    bool _isCheckTaskRunning;
    bool _isUpdateTaskRunning;
};
