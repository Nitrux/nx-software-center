//
// Created by alexis on 23/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_REMOVEAPPIMAGEINTERACTOR_H
#define NOMAD_SOFTWARE_CENTER_REMOVEAPPIMAGEINTERACTOR_H

#include <QVariantMap>
#include <QFile>

#include "TaskMetadata.h"
#include "Interactor.h"
#include "entities/Application.h"

class RemoveAppImageInteractor : public Interactor {
Q_OBJECT
    const Application application;
    const QStringList files;
public:
    RemoveAppImageInteractor(const Application &application, const QStringList &files, QObject *parent = nullptr);


    void execute() override;

protected:
    void setRunningMetadata();

    void setCompletedMetadata();

    void setFailedMetadata();

};


#endif //NOMAD_SOFTWARE_CENTER_REMOVEAPPIMAGEINTERACTOR_H
