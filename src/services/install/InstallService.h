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

private:
    Q_SLOT

    QDir _applicationsDir;
    QDir _partialsDir;
    QQueue<QUrl> _installQueue;
};
