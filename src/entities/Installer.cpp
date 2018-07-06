//
// Created by alexis on 7/4/18.
//

#include <QtCore/QDir>
#include "InstallTask.h"
#include "Installer.h"

Installer::Installer()
        :QObject(), restClient(nullptr) { }

void Installer::setRestClient(RestClient* restClient)
{
    Installer::restClient = restClient;
}
InstallTask* Installer::buildInstallLatestReleaseTask(const QString& id)
{
    auto task = new InstallTask();
    task->setId(id);
    task->setRestClient(restClient);
    task->setApplicationsDir(QDir::homePath() + "/Applications");

    return task;
}
