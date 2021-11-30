#pragma once
// system

// libraries
#include <MauiKit/FileBrowsing/downloader.h>
#include <QObject>

// local
#include "TaskManager.h"
#include "nx.h"
#include "utils/appimagetools.h"

namespace FMH {
    class Downloader;
}

class DownloadTask : public Task
{
    Q_OBJECT
public:
    DownloadTask(const QString &id, const QString &appName, const QUrl &appDownloadUrl, QObject *parent);
    Q_SCRIPTABLE void start() override;

protected:
    // Q_SLOT void onWorkerStarted(short);
    Q_SLOT void onWorkerFinished(QString path);
    Q_SLOT void onWorkerProgress(const int &progress);
    Q_SLOT void onWorkerError(QString warning);

private:
    QString _appName;
    QUrl _appDownloadUrl;

    FMH::Downloader *_worker;
};