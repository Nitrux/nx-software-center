#ifndef SEARCHAPPLICATIONSINTERACTOR_H
#define SEARCHAPPLICATIONSINTERACTOR_H

#include <QList>
#include "interactor.h"

class Registry;
class Repository;
class SearchApplicationsInteractorListener;

/***
 * See the following link for usecase details
 * https://github.com/nomad-desktop/nx-software-center/wiki/Use-Case:-Search-Applications
 ***/
class SearchApplicationsInteractor : public Interactor
{
public:
    SearchApplicationsInteractor(const QString &searchString,
                                 Registry *registry,
                                 QList<Repository *> repositories,
                                 SearchApplicationsInteractorListener * listener);
    virtual ~SearchApplicationsInteractor();

    virtual void execute() override;

protected:
    QString m_searchString;
    Registry * m_registry = nullptr;
    QList<Repository *> m_repositories;
    SearchApplicationsInteractorListener * m_listener;
};

#endif // SEARCHAPPLICATIONSINTERACTOR_H
