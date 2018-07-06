//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_INSTALLER_H
#define NX_SOFTWARE_CENTER_INSTALLER_H

#include <QObject>
#include <QNetworkAccessManager>

#include <gateways/RestClient.h>

class InstallTask;
class Installer : public QObject {
Q_OBJECT
    RestClient* restClient;

public:
    Installer();

    void setRestClient(RestClient* restClient);
    InstallTask* buildInstallLatestReleaseTask(const QString& id);
};

#endif //NX_SOFTWARE_CENTER_INSTALLER_H
