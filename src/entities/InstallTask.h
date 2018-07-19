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

class InstallTask : public Task {
Q_OBJECT
    ApplicationRepository *repository;
    QString id;
    QString channel;
    QString applicationsDir;
    bool running;

    ApplicationRepositoryGet *applicationRepositoryGet;
    FileDownload *fileDownload;
protected:
    friend class Installer;

    InstallTask();

    void setId(const QString &id);

    void setRepository(ApplicationRepository *restClient);

public:
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);

public:
    void setApplicationsDir(const QString &applicationsDir);

    const QString &getChannel() const;

    void setChannel(const QString &channel);

    void start() override;

    void stop() override;

signals:

    void completed();

    void failed(const QString &reason);

protected:
    void getApplicationInfo();

protected slots:

    void handleGetApplicationInfoCompleted();

    void handleGetApplicationInfoFailed(const QString &reason);

    void handleFileDownloadCompleted();

    void handleFileDownloadProgress(qint64 progress, qint64 total, const QString &message);

    QString getDownloadFilePath(const ApplicationFull &applicationInfo, const ApplicationFull::Release &r,
                                const ApplicationFull::File &file) const;

    void downloadApplicationFile(ApplicationFull &applicationInfo);

    void registerAppImage(const QString &filePath);

    void fail(const QString &msg);
};

#endif //NX_SOFTWARE_CENTER_INSTALLTASK_H
