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
    RemoveAppImageInteractor(const Application &application, const QStringList &files, QObject *parent = nullptr) :
            Interactor(parent), application(application), files(files) {

        metadata.insert(TaskMetadata::KEY_APP_ID, application.getId());
        metadata.insert(TaskMetadata::KEY_APP_CODENAME, application.getCodeName());
        metadata.insert(TaskMetadata::KEY_APP_VERSION, application.getVersion());
        metadata.insert(TaskMetadata::KEY_APP_NAME, application.getName());
        metadata.insert(TaskMetadata::KEY_APP_AUTHOR, application.getAuthors().join(", "));

        QString d = QString("Removing %1 %2").arg(application.getCodeName(), application.getVersion());
        metadata.insert(TaskMetadata::KEY_DESCRIPTION, d);
        metadata.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_CREATED);
        metadata.insert(TaskMetadata::KEY_TYPE, TaskMetadata::VALUE_TYPE_REMOVE);
    }


    void execute() override {
        setRunningMetadata();

        bool error = false;
        QStringList errors;
        for (const QString &path: files) {
            QFile f(path);
            if (!f.remove()) {
                error = true;
                errors << "";
            }

        }
        if (error)
            setFailedMetadata();
        else
            setCompletedMetadata();

        emit completed();
    }

protected:
    void setRunningMetadata() {
        QVariantMap statusUpdate;
        statusUpdate.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_RUNNING);
        setMetadata(statusUpdate);
    }

    void setCompletedMetadata() {
        QVariantMap statusUpdate;
        statusUpdate.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_COMPLETED);
        setMetadata(statusUpdate);
    }

    void setFailedMetadata() {
        QVariantMap statusUpdate;
        statusUpdate.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_FAILED);
        setMetadata(statusUpdate);
    }

};


#endif //NOMAD_SOFTWARE_CENTER_REMOVEAPPIMAGEINTERACTOR_H
