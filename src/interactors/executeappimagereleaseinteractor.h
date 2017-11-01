#ifndef EXECUTEAPPIMAGERELEASEINTERACTOR_H
#define EXECUTEAPPIMAGERELEASEINTERACTOR_H

#include <QString>

#include "interactor.h"

class Registry;
class ExecuteAppImageReleaseInteractorListener;
class ExecuteAppImageReleaseInteractor : public Interactor
{
public:
    ExecuteAppImageReleaseInteractor(QString appId, QString releaseId,
                                     Registry *registry,
                                     ExecuteAppImageReleaseInteractorListener *listener);
    virtual void execute() override;

private:
    QString m_appId;
    QString m_releaseId;
    Registry *m_registry;
    ExecuteAppImageReleaseInteractorListener *m_listener = nullptr;
};

#endif // EXECUTEAPPIMAGERELEASEINTERACTOR_H
