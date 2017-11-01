#include "taskexecuteappimagecontroller.h"

#include "../interactors/executeappimagereleaseinteractor.h"

TaskExecuteAppImageController::TaskExecuteAppImageController(QString appId, QString releaseId, Registry *registry, QObject *parent): TaskController(parent)
{
    Q_ASSERT(registry != nullptr);
    m_interactor = new ExecuteAppImageReleaseInteractor(appId, releaseId, registry, this);
}

TaskExecuteAppImageController::~TaskExecuteAppImageController()
{

}

void TaskExecuteAppImageController::run()
{
    m_state = TASK_RUNNING;
    emit stateChanged(m_state);

    m_interactor->execute();
}

void TaskExecuteAppImageController::success()
{
    m_state = TASK_COMPLETED;
    emit stateChanged(m_state);
}

void TaskExecuteAppImageController::fail(QString message)
{
    m_state = TASK_FAILED;
    m_message = message;
    emit stateChanged(m_state);
    emit messageChanged(m_message);
}
