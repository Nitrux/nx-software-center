#ifndef TASKREMOVEAPPIMAGERELEASECONTROLLER_H
#define TASKREMOVEAPPIMAGERELEASECONTROLLER_H

#include "taskcontroller.h"
#include "../interactors/removeappimagereleaseinteractorlistener.h"

class Registry;
class Interactor;
class TaskRemoveAppImageReleaseController : public TaskController, RemoveAppImageReleaseInteractorListener
{
    Q_OBJECT
public:
    TaskRemoveAppImageReleaseController(QString appId, QString releaseId,
                                        Registry *registry, QObject *parent = nullptr);

    Q_INVOKABLE virtual void run() override;
    virtual void finished() override;
    virtual void error(const QString &errorMessage) override;

protected:
    Interactor *m_interactor = nullptr;
};

#endif // TASKREMOVEAPPIMAGERELEASECONTROLLER_H
