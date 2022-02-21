#pragma once
// system

// libraries
#include <QDir>
#include <QObject>
#include <QQueue>
#include <QUrl>
#include <ResponseDTO/pling_store_application_data.h>

// local
#include "services/Application.h"
#include "services/TaskData.h"

class InstallService : public QObject
{
    Q_OBJECT
public:
    explicit InstallService(const QString &applicationsDir, QObject *parent = nullptr);
    Q_SCRIPTABLE void installFromPlingStore(const QUrl &appimageUrl, PlingStoreApplicationData *plingStoreApplicationData);

    // use it to forward tasks updates
    Q_SIGNAL void progressNotification(const TaskData &update);

    // update desktop integration
    Q_SLOT void handleApplicationAdded(const Application &application);
    Q_SLOT void handleApplicationUpdated(const Application &application);
    Q_SLOT void handleApplicationRemoved(const Application &application);

private:
    QDir _applicationsDir;
    QDir _partialsDir;
    QQueue<QUrl> _installQueue;
    void installMainBundle(const Application &applicationData) const;
    void uninstallAllBundles(const Application &applicationData) const;
};
