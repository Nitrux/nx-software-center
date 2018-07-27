//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_INSTALLTASK_H
#define NX_SOFTWARE_CENTER_INSTALLTASK_H

#include <QObject>
#include <gateways/ApplicationRepositoryRestClient.h>
#include "ApplicationFull.h"
#include "Task.h"

class ApplicationRepository;
class ApplicationRepositoryGet;
class FileDownload;

class DeployTask : public Task {
Q_OBJECT
    ApplicationRepository *repository;
    QString id;
    QString channel;
    QString applicationsDir;
    bool running;

    ApplicationRepositoryGet *applicationRepositoryGet;
    FileDownload *fileDownload;
protected:
    friend class Deployer;

    DeployTask();

    void setId(const QString &id);

    void setRepository(ApplicationRepository *restClient);

public:
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

protected:
    void getApplicationInfo();

    QString getDownloadFilePath(AppImageInfo applicationInfo, const ApplicationFull::Release &r,
                                const ApplicationFull::File &file) const;

    void downloadApplicationFile(AppImageInfo applicationInfo);

    void registerAppImage(const QString &filePath);

    void fail(const QString &msg);
};

#endif //NX_SOFTWARE_CENTER_INSTALLTASK_H
