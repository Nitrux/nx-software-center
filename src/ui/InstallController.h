#ifndef INSTALLCONTROLLER_H
#define INSTALLCONTROLLER_H

#include <QObject>

#include "entities/Repository.h"
#include "entities/Executor.h"
#include "gateways/DownloadManager.h"

class InstallController : public QObject
{
    Q_OBJECT
    Repository *repository;
    Executor *executor;
    DownloadManager *downloadManager;
public:
    explicit InstallController(Repository *repository, Executor *executor, DownloadManager *downloadManager,
                                   QObject *parent = nullptr);

signals:

public slots:
    void install(const QString &application_id);
};

#endif // INSTALLCONTROLLER_H
