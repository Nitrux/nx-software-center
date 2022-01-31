#pragma once
// system

// libraries
#include <QDir>
#include <QObject>
#include <QQueue>
#include <QUrl>
#include <ResponseDTO/application.h>

// local
#include "services/ApplicationData.h"
#include "services/TaskData.h"

class InstallService : public QObject
{
    Q_OBJECT
public:
    explicit InstallService(const QString &applicationsDir, QObject *parent = nullptr);
    Q_SCRIPTABLE void installFromPlingStore(const QUrl &appimageUrl, Application *plingStoreApplicationData);

    // use it to forward tasks updates
    Q_SIGNAL void progressNotification(const TaskData &update);

    // update desktop integration
    Q_SLOT void handleApplicationAdded(const ApplicationData &application);
    Q_SLOT void handleApplicationUpdated(const ApplicationData &application);
    Q_SLOT void handleApplicationRemoved(const ApplicationData &application);

private:
    QDir _applicationsDir;
    QDir _partialsDir;
    QQueue<QUrl> _installQueue;
    void installMainBundle(const ApplicationData &applicationData) const;
    void uninstallAllBundles(const ApplicationData &applicationData) const;
};
