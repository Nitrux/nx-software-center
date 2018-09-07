//
// Created by alexis on 2/24/18.
//

#include <QtCore/QFileInfo>
#include <QtCore/QProcess>

#include "RunController.h"


void RunController::run(const QString &id) {
    AppImageInfo appImageInfo = deployedApplicationsRegistry->getLatestDeployedVersionOf(id);
    runAppImage(appImageInfo.file.path);
}

void RunController::runAppImage(const QString &path) {
    QFileInfo fi(path);
    QProcess::startDetached(fi.absoluteFilePath(), {}, fi.absolutePath());
}

RunController::RunController(QObject *parent) : QObject(parent),
                                                deployedApplicationsRegistry(nullptr) {}

void RunController::setDeployedApplicationsRegistry(DeployedApplicationsRegistry *deployedApplicationsRegistry) {
    RunController::deployedApplicationsRegistry = deployedApplicationsRegistry;
}

