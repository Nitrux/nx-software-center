#ifndef REMOVEAPPIMAGERELEASEINTERACTOR_H
#define REMOVEAPPIMAGERELEASEINTERACTOR_H

#include "interactor.h"

#include <QString>

class Registry;
class RemoveAppImageReleaseInteractorListener;
class RemoveAppImageReleaseInteractor : public Interactor
{
public:
    RemoveAppImageReleaseInteractor(QString appId, QString releaseId,
                                    Registry *registry,
                                    RemoveAppImageReleaseInteractorListener *listener);

    virtual void execute() override;

protected:
    QString m_appId;
    QString m_releaseId;
    Registry *m_registry;
    RemoveAppImageReleaseInteractorListener *m_listener;
};

#endif // REMOVEAPPIMAGERELEASEINTERACTOR_H
