//
// Created by alexis on 23/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_UPGRADER_H
#define NOMAD_SOFTWARE_CENTER_UPGRADER_H

#include <QObject>
#include <QPair>
#include <QList>
#include "Remover.h"
#include "Deployer.h"
#include "../tasks/UpgradeTask.h"
#include <gateways/ApplicationRepository.h>
#include <gateways/DeployedApplicationsRegistry.h>

class Upgrader : public QObject {
Q_OBJECT
    ApplicationRepository *applicationRepository;
    DeployedApplicationsRegistry *deployedApplicationsRegistry;
    Deployer *deployer;
    Remover *remover;

    QList<AppImageInfo> upgradableApplications;

    int pendingRequestCount;
    QList<ApplicationRepositoryGet*> getApplicationsRequests;

public:
    explicit Upgrader(QObject *parent = nullptr);

    void setApplicationRepository(ApplicationRepository *applicationRepository);

    void setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry);

    void setDeployer(Deployer *deployer);

    void setRemover(Remover *remover);

    void lookUpForUpgrades();

    const QList<AppImageInfo> &getUpgradableApplications() const;

    UpgradeTask *buildUpgradeTask(const QString &id);
signals:
    void upgradesLookUpCompleted();

protected slots:
    void handleApplicationRepositoryGetResult();
};


#endif //NOMAD_SOFTWARE_CENTER_UPGRADER_H
