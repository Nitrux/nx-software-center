#include "InstallController.h"
#include <QDebug>
#include "interactors/InstallAppImageInteractor.h"
#include <QSharedPointer>

InstallController::InstallController(Repository *repository, Executor *executor, DownloadManager *downloadManager,
                                     QObject *parent)
    : QObject(parent), repository(repository), executor(executor), downloadManager(downloadManager) {

}

void InstallController::install(const QString &application_id)
{
    qDebug() << "Downloading " << application_id;

    const Application &a = repository->get(application_id);
    InstallAppImageInteractor *i = new InstallAppImageInteractor(a, downloadManager);

    executor->execute(i);
}
