//
// Created by alexis on 7/4/18.
//

#include <QtCore/QDir>
#include "InstallTask.h"
#include "Installer.h"

Installer::Installer()
        :QObject(), repository(nullptr) { }

void Installer::setRepository(ApplicationRepository *restClient)
{
    Installer::repository = restClient;
}
InstallTask* Installer::buildInstallLatestReleaseTask(const QString& id)
{
    auto task = new InstallTask();
    task->setId(id);
    task->setRepository(repository);
    task->setApplicationsDir(QDir::homePath() + "/Applications");

    return task;
}
