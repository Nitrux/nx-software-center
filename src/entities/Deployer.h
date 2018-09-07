//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_INSTALLER_H
#define NX_SOFTWARE_CENTER_INSTALLER_H

#include <QNetworkAccessManager>
#include <gateways/ApplicationRepository.h>
#include <gateways/DeployedApplicationsRegistry.h>

class DeployTask;

class Deployer {
    ApplicationRepository *repository;
    DeployedApplicationsRegistry *deployedApplicationsRegistry;

public:
    Deployer();

    void setRepository(ApplicationRepository *repository);

    void setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry);

    DeployTask *buildDeployLatestReleaseTask(const QString &id);
    DeployTask *buildDeployTask(AppImageInfo appImageInfo);
};

#endif //NX_SOFTWARE_CENTER_INSTALLER_H
