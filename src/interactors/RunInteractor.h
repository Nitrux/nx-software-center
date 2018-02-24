//
// Created by alexis on 2/24/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_RUNINTERACTOR_H
#define NOMAD_SOFTWARE_CENTER_RUNINTERACTOR_H

#include "Interactor.h"

class RunInteractor : public Interactor {
    QString target;
public:
    RunInteractor(const QString &target, QObject *parent);


public slots:

    virtual void execute();;

};


#endif //NOMAD_SOFTWARE_CENTER_RUNINTERACTOR_H
