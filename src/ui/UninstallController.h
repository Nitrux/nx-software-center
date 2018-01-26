//
// Created by alexis on 23/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_UNINSTALLCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_UNINSTALLCONTROLLER_H

#include <QObject>
#include <entities/Repository.h>

#include "entities/Registry.h"
#include "entities/Executor.h"

class UninstallController : public QObject {
Q_OBJECT

    Repository *repository;
    Registry *registry;
    Executor *executor;
public:
    UninstallController(Repository *repository, Registry *registry, Executor *executor,
                            QObject *parent = nullptr);

public slots:
    void uninstall(const QString &appId);
};


#endif //NOMAD_SOFTWARE_CENTER_UNINSTALLCONTROLLER_H
