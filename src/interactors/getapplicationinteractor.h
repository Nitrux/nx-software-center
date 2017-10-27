#ifndef GETAPPLICATIONINTERACTOR_H
#define GETAPPLICATIONINTERACTOR_H

#include "interactor.h"
#include "getapplicationinteractorlistener.h"

class Repository;
class GetApplicationInteractor : public Interactor
{
public:
    GetApplicationInteractor(QString application_id, QString release_id,
                             QList<Repository *> repositories,
                             GetApplicationInteractorListener *listener);

    virtual ~GetApplicationInteractor();

    virtual void execute();
protected:
    QString m_application_id;
    QString m_release_id;
    QList<Repository *> m_repositories;
    GetApplicationInteractorListener *m_listener;
};

#endif // GETAPPLICATIONINTERACTOR_H
