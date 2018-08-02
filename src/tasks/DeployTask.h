//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_INSTALLTASK_H
#define NX_SOFTWARE_CENTER_INSTALLTASK_H

#include <QObject>
#include <QFutureWatcher>
#include <gateways/DeployedApplicationsRegistry.h>
#include <gateways/ApplicationRepositoryRestClient.h>
#include "entities/Application.h"
#include "entities/Task.h"

class ApplicationRepository;

class ApplicationRepositoryGet;

class FileDownload;

class DeployTask : public Task {
Q_OBJECT
    ApplicationRepository *repository;
    DeployedApplicationsRegistry *deployedApplicationsRegistry;
    QString id;
    QString channel;
    QString applicationsDir;
    QFutureWatcher<QString> appImageCheckSumFutureWatcher;
    bool running;

    ApplicationRepositoryGet *applicationRepositoryGet;
    FileDownload *fileDownload;


public:
    DeployTask();

    void setId(const QString &id);

    void setRepository(ApplicationRepository *restClient);

    void setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry);

    void setApplicationsDir(const QString &applicationsDir);

    const QString &getChannel() const;

    void setChannel(const QString &channel);

    void start() override;

    void stop() override;

protected slots:

    void handleGetApplicationInfoCompleted();

    void handleGetApplicationInfoFailed(const QString &reason);

    void handleFileDownloadStopped();

    void handleFileDownloadCompleted();

    void handleFileDownloadProgress(qint64 progress, qint64 total, const QString &message);

    void handleAppImageCheckSumFutureFinished();

protected:
    void getApplicationInfo();

    QString getDownloadFilePath(AppImageInfo applicationInfo, const Application::Release &r,
                                const Application::File &file) const;

    void downloadApplicationFile(AppImageInfo applicationInfo);

    void registerAppImage(const QString &filePath);

    void fail(const QString &msg);

    void verifyAppImageFile(QString path);

    void complete();
};

#endif //NX_SOFTWARE_CENTER_INSTALLTASK_H
