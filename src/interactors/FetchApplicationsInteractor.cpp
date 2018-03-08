//
// Created by alexis on 22/12/17.
//

#include "FetchApplicationsInteractor.h"

#include <QDebug>
#include <QtCore/QThread>

#include "entities/Application.h"
#include "entities/Source.h"
#include "TaskMetadata.h"

FetchApplicationsInteractor::FetchApplicationsInteractor(QList<Source *> sources, QObject *parent) : Interactor(
        parent), sources(sources), runningTasks(0) {

    metadata.insert(TaskMetadata::KEY_DESCRIPTION, "Fetching applications");
    metadata.insert(TaskMetadata::KEY_STATUS, TaskMetadata::VALUE_STATUS_CREATED);
    metadata.insert(TaskMetadata::KEY_TYPE, TaskMetadata::VALUE_TYPE_UPDATE);
}

QList<Application> FetchApplicationsInteractor::getResults() const {
    return results;
}

QStringList FetchApplicationsInteractor::getErrors() const {
    return errors;
}

void FetchApplicationsInteractor::execute() {
    if (runningTasks != 0)
        qWarning() << "Calling execute when the interactor is still bussy.";

    runningTasks = sources.size();
    for (Source *source : sources) {
        connect(source, &Source::fetchedAllApplications, this, &FetchApplicationsInteractor::handleFetchedApplications);
        connect(source, &Source::fetchError, this, &FetchApplicationsInteractor::handleFetchError);
        source->fetchAllApplications();
    }
}

void FetchApplicationsInteractor::handleFetchedApplications(const QList<Application> applications) {
    QObject *source = sender();
    disconnect(source, nullptr, this, nullptr);

    results.append(applications);
    runningTasks--;

    if (isCompleted()) {
        emit completed();
        setCompletedMetadata();
    }

}

void FetchApplicationsInteractor::setCompletedMetadata() {
    setMetadata({{TaskMetadata::KEY_DESCRIPTION, "Fetch completed"},
                 {TaskMetadata::KEY_STATUS,      TaskMetadata::VALUE_STATUS_COMPLETED}});
}

void FetchApplicationsInteractor::handleFetchError(const QString &error) {
    QObject *source = sender();
    disconnect(source, nullptr, this, nullptr);

    errors.append(error);
    runningTasks--;

    if (isCompleted())
            emit completed();
}

bool FetchApplicationsInteractor::isCompleted() {
    return runningTasks <= 0;
}