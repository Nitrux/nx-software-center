//
// Created by alexis on 7/24/18.
//

#ifndef NX_SOFTWARE_CENTER_REMOVER_H
#define NX_SOFTWARE_CENTER_REMOVER_H

#include <QObject>
#include <gateways/DeployedApplicationsRegistry.h>
#include "tasks/RemoveTask.h"

class Remover : public  QObject{
    DeployedApplicationsRegistry *registry;

public:
    void setRegistry(DeployedApplicationsRegistry *registry);

    RemoveTask * buildRemoveTaskByPath(const QString &filePath);
    RemoveTask * buildRemoveTaskById(const QString &id);
};


#endif //NX_SOFTWARE_CENTER_REMOVER_H
