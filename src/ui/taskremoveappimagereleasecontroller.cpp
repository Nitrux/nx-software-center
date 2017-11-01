#include "taskremoveappimagereleasecontroller.h"

#include <QtConcurrent>

#include "../interactors/removeappimagereleaseinteractor.h"

TaskRemoveAppImageReleaseController::TaskRemoveAppImageReleaseController(QString appId, QString releaseId, Registry *registry, QObject *parent) : TaskController(parent)
{
    Q_ASSERT(registry != nullptr);
    m_interactor = new RemoveAppImageReleaseInteractor(appId,
                                                       releaseId,
                                                       registry,
                                                       this);

}

void TaskRemoveAppImageReleaseController::run()
{
    if (m_state == TASK_CREATED)
    {
        QtConcurrent::run([=](){
            m_interactor->execute();
        });
        emit stateChanged(m_state);
    }
}

void TaskRemoveAppImageReleaseController::finished()
{
    m_state = TASK_COMPLETED;
    emit stateChanged(m_state);
}

void TaskRemoveAppImageReleaseController::error(const QString &errorMessage)
{
    m_state = TASK_FAILED;
    m_message = errorMessage;

    emit stateChanged(m_state);
    emit messageChanged(m_message);
    emit finished();
}
