//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_INSTALLTASK_H
#define NX_SOFTWARE_CENTER_INSTALLTASK_H

#include <QObject>
#include <gateways/RestClient.h>
#include "ApplicationFull.h"

class GetApplicationRequest;
class FileDownload;
class InstallTask : public QObject {
Q_OBJECT
    RestClient* restClient;
    QString id;
    QString channel;
    QString applicationsDir;
    bool running;

    GetApplicationRequest* getApplicationRequest;
    FileDownload *fileDownload;
protected:
    friend class Installer;
    InstallTask();
    void setRestClient(RestClient* restClient);
    void setId(const QString& id);
public:
    void setApplicationsDir(const QString& applicationsDir);

public:
    const QString& getChannel() const;
    void setChannel(const QString& channel);

public slots:
    void start();

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
