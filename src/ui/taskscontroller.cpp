#include "taskscontroller.h"

#include <QtConcurrent>

#include "../interactors/downloadappimagereleaseinteractor.h"
#include "../interactors/removeappimagereleaseinteractor.h"

TasksController::TasksController(QList<Repository *> repositories, Registry *registry, DownloadManager *downloadManager, QObject *parent) : QObject(parent)
{
    Q_ASSERT(registry != nullptr);
    m_registry = registry;
    Q_ASSERT(downloadManager != nullptr);
    m_downladManager = downloadManager;

    m_repositories = repositories;

}

void TasksController::progress(const int progress, const int total, const QString statusMessage)
{
    emit taskProgressUpdate(progress, total, statusMessage);
}

void TasksController::downloadComplete(const QString )
{
    emit taskComplete();
}

void TasksController::finished()
{
    emit taskComplete();
}

void TasksController::error(const QString &errorMessage)
{
    emit taskError(errorMessage);
}

void TasksController::download(QString appId, QString releaseId)
{
    if (m_busy)
    {
        emit systemBusy();
        return;
    }

    m_busy = true;
    m_currentTask = new DownloadAppImageReleaseInteractor(appId, releaseId,
                                                          m_repositories,
                                                          m_downladManager,
                                                          m_registry,
                                                          this);

    QtConcurrent::run([=](){
        m_currentTask->execute();
        m_currentTask = nullptr;
        m_busy = false;
    });
}

void TasksController::remove(QString appId, QString releaseId)
{
    if (m_busy)
    {
        emit systemBusy();
        return;
    }

    m_busy = true;
    m_currentTask = new RemoveAppImageReleaseInteractor(appId, releaseId,
                                                        m_registry, this);

    QtConcurrent::run([=](){
        m_currentTask->execute();
        m_currentTask = nullptr;
        m_busy = false;
    });
}
