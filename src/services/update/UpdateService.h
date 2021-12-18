#pragma once
// system

// libraries
#include <QList>
#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QThread>

// local
#include "UpdateCheckData.h"
#include "UpdatesWorker.h"
#include "services/TaskManager.h"

class UpdateService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int updatesAvailable READ getUpdatesAvailable NOTIFY updatesAvailableChanged)

public:
    explicit UpdateService(QObject *parent);
    ~UpdateService() override;

    Q_SLOT int getUpdatesAvailable() const;
    Q_SIGNAL void updatesAvailableChanged(int updatesAvailable);

    void checkUpdates(const QList<ApplicationData> &applications);

    void update(const QList<ApplicationData> &applications);

    // notifies when a new bundle is available
    Q_SIGNAL void updateDownloaded(const ApplicationBundle &bundle);

    // use it to forward tasks updates
    Q_SIGNAL void taskUpdate(QVariantMap update);

private:
    Q_SLOT void handleUpdateInformation(const UpdateCheckData &updateInformation);
    Q_SLOT void handleUpdateCompleted(const ApplicationBundle &bundle);

    void refreshUpdatesAvailable();
    int _updatesAvailable;

    QMap<QString, UpdateCheckData> _updatesInformation; // app id - UpdateInformation
    QThread _workerThread;
    UpdatesWorker _checkUpdatesWorker;
};
