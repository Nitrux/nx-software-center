//
// Created by alexis on 22/12/17.
//

#include "FetchApplicationsInteractor.h"

#include <QDebug>

#include "Application.h"
#include "Source.h"

FetchApplicationsInteractor::FetchApplicationsInteractor(QList<Source *> sources, QObject *parent) : Interactor(
        parent) {
    this->sources = sources;
    runningTasks = 0;
}

void FetchApplicationsInteractor::execute() {
    if (runningTasks != 0)
        qWarning() << "Calling execute when the interactor is still bussy.";

    for (Source *source : sources) {
        runningTasks++;
        connect(source, &Source::fetchedAllApplications, this, &FetchApplicationsInteractor::handleFetchedApplications);
        connect(source, &Source::fetchError, this, &FetchApplicationsInteractor::handleFetchError);
        source->fetchAllApplications();
    }
}

void FetchApplicationsInteractor::handleFetchedApplications(QList<Application> applications) {
    results.append(applications);
    runningTasks--;
    if (isCompleted())
            emit completed(results, errors);
}

void FetchApplicationsInteractor::handleFetchError(const QString &error) {
    errors.append(error);
    runningTasks--;

    if (isCompleted())
            emit completed(results, errors);
}

bool FetchApplicationsInteractor::isCompleted() {
    return runningTasks == 0;
}