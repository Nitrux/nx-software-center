//
// Created by alexis on 23/01/18.
//

#include "RemoveAppImageInteractor.h"

RemoveAppImageInteractor::RemoveAppImageInteractor(const Application &application, const QStringList &files, QObject *parent) :
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

void RemoveAppImageInteractor::execute() {
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

void RemoveAppImageInteractor::setRunningMetadata() {
    QVariantMap statusUpdate;
    statusUpdate.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_RUNNING);
    setMetadata(statusUpdate);
}

void RemoveAppImageInteractor::setCompletedMetadata() {
    QVariantMap statusUpdate;
    statusUpdate.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_COMPLETED);
    setMetadata(statusUpdate);
}

void RemoveAppImageInteractor::setFailedMetadata() {
    QVariantMap statusUpdate;
    statusUpdate.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_FAILED);
    setMetadata(statusUpdate);
}
