//
// Created by alexis on 7/24/18.
//

#ifndef NX_SOFTWARE_CENTER_REMOVER_H
#define NX_SOFTWARE_CENTER_REMOVER_H

#include <QObject>
#include <gateways/DeployedApplicationsRegistry.h>
#include "RemoveTask.h"

class Remover : public  QObject{
    DeployedApplicationsRegistry *registry;

public:
    void setRegistry(DeployedApplicationsRegistry *registry);

    RemoveTask * buildRemoveTask(const QString &filePath);
};


#endif //NX_SOFTWARE_CENTER_REMOVER_H
