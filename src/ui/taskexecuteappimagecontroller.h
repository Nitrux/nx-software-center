#ifndef TASKEXECUTEAPPIMAGECONTROLLER_H
#define TASKEXECUTEAPPIMAGECONTROLLER_H

#include "taskcontroller.h"
#include "../interactors/executeappimagereleaseinteractorlistener.h"

class Registry;
class Interactor;
class TaskExecuteAppImageController : public TaskController, ExecuteAppImageReleaseInteractorListener
{
public:
    TaskExecuteAppImageController(QString appId, QString releaseId,
                                  Registry *registry,
                                  QObject *parent);

    virtual ~TaskExecuteAppImageController();

    Q_INVOKABLE virtual void run() override;

    virtual void success() override;
    virtual void fail(QString message) override;

protected:
    Interactor *m_interactor = nullptr;
};

#endif // TASKEXECUTEAPPIMAGECONTROLLER_H
