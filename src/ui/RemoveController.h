//
// Created by alexis on 23/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_UNINSTALLCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_UNINSTALLCONTROLLER_H

#include <QObject>
#include <gateways/DeployedApplicationsRegistry.h>

#include "entities/Remover.h"
#include "entities/Worker.h"

class RemoveController : public QObject {
Q_OBJECT

    Remover * remover;
    Worker * worker;

public:
    RemoveController(QObject *parent = nullptr);

    void setRemover(Remover *remover);

    void setWorker(Worker *worker);

public slots:

    void removeAppImage(const QString &path);
    void remove(const QString &id);
};


#endif //NOMAD_SOFTWARE_CENTER_UNINSTALLCONTROLLER_H
