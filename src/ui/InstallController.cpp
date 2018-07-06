#include <QList>
#include "InstallController.h"

InstallController::InstallController(Installer* installer, QObject* parent)
        :QObject(parent), installer(installer) { }

void InstallController::install(const QString& application_id)
{
    auto task = installer->buildInstallLatestReleaseTask(application_id);
    tasks << task;
    if (tasks.size()==1)
        startTask(task);

}
void InstallController::startTask(InstallTask* task) const
{
    connect(task, &InstallTask::completed, this, &InstallController::handleInstallTaskCompleted);
    task->start();
}
void InstallController::handleInstallTaskCompleted()
{
    if (!tasks.isEmpty()) {
        auto finishedTask = tasks.front();
        tasks.pop_front();

        disconnect(finishedTask, nullptr, nullptr, nullptr);
        finishedTask->deleteLater();
    }

    if (tasks.size()>0)
        startTask(tasks.first());
}
