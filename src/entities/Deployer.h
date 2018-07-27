//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_INSTALLER_H
#define NX_SOFTWARE_CENTER_INSTALLER_H

#include <QNetworkAccessManager>
#include <gateways/ApplicationRepository.h>

class DeployTask;

class Deployer {
    ApplicationRepository *repository;
public:
    Deployer();

    void setRepository(ApplicationRepository *repository);

    DeployTask *buildInstallLatestReleaseTask(const QString &id);
};

#endif //NX_SOFTWARE_CENTER_INSTALLER_H
