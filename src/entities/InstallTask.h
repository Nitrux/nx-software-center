//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_INSTALLTASK_H
#define NX_SOFTWARE_CENTER_INSTALLTASK_H

#include <QObject>
#include <gateways/RestClient.h>
#include "ApplicationFull.h"
#include "Task.h"

class GetApplicationRequest;
class FileDownload;
class InstallTask : public Task {
Q_OBJECT
    RestClient* restClient;
    QString id;
    QString channel;
    QString applicationsDir;
    bool running;

    GetApplicationRequest* getApplicationRequest;
    FileDownload* fileDownload;
protected:
    friend class Installer;
    InstallTask();
    void setRestClient(RestClient* restClient);
    void setId(const QString& id);
public:
    void setApplicationsDir(const QString& applicationsDir);
    const QString& getChannel() const;
    void setChannel(const QString& channel);

    void start() override;
    void stop() override;

signals:
    void completed();
    void failed(const QString& reason);

protected:
    void getApplicationInfo();
protected slots:
    void handleGetApplicationInfoCompleted();
    void handleGetApplicationInfoFailed(const QString& reason);
    void handleFileDownloadCompleted();

    QString getDownloadFilePath(const ApplicationFull& applicationInfo, const ApplicationFull::Release& r,
            const ApplicationFull::File& file) const;
    void downloadApplicationFile(ApplicationFull& applicationInfo);
    void registerAppImage(const QString& filePath);
};

#endif //NX_SOFTWARE_CENTER_INSTALLTASK_H
