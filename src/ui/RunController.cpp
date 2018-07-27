//
// Created by alexis on 2/24/18.
//

#include <interactors/RunInteractor.h>
#include <QtCore/QFileInfo>

#include "RunController.h"


void RunController::run(const QString &id) {
    AppImageInfo appImageInfo = deployedApplicationsRegistry->getLatestDeployedVersionOf(id);
    spawnRunInteractor(appImageInfo.file.path);
}

void RunController::runAppImage(const QString &path) {
    RunInteractor *i = new RunInteractor(path, this);

    connect(i, &RunInteractor::completed, [i]() { i->deleteLater(); });
    i->execute();
}

void RunController::spawnRunInteractor(const QString &executable) {
    RunInteractor *i = new RunInteractor(executable, this);

    connect(i, &RunInteractor::completed, [i]() { i->deleteLater(); });
    i->execute();
}

RunController::RunController(QObject *parent) : QObject(parent),
                                                deployedApplicationsRegistry(nullptr) {}

void RunController::setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry) {
    RunController::deployedApplicationsRegistry = deployedApplicationsRegistry;
}

