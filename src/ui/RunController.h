//
// Created by alexis on 2/24/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_RUNCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_RUNCONTROLLER_H

#include <QObject>
#include <entities/TaskLogger.h>
#include <gateways/DeployedApplicationsRegistry.h>

class RunController : public QObject {
Q_OBJECT
    DeployedApplicationsRegistry *deployedApplicationsRegistry;
public:
    RunController(QObject *parent = nullptr);

    void setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry);

public slots:

    void run(const QString &id);

    void runAppImage(const QString &path);

};


#endif //NOMAD_SOFTWARE_CENTER_RUNCONTROLLER_H
