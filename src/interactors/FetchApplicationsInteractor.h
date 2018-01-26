//
// Created by alexis on 22/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_FETCHAPPLICATIONS_H
#define NOMAD_SOFTWARE_CENTER_FETCHAPPLICATIONS_H

#include <QList>
#include <QString>

#include "interactors/Interactor.h"

class Source;

class Application;

class FetchApplicationsInteractor : public Interactor {
Q_OBJECT
    int runningTasks;
    QList<Source *> sources;
    QList<Application> results;
    QStringList errors;
public:
    explicit FetchApplicationsInteractor(QList<Source *> sources, QObject *parent = nullptr);

    QList<Application> getResults() const;
    QStringList getErrors() const;

public slots:

    void execute() override;

protected slots:

    void handleFetchedApplications(QList<Application> applications);

    void handleFetchError(const QString &error);


private:
    bool isCompleted();
};


#endif //NOMAD_SOFTWARE_CENTER_FETCHAPPLICATIONS_H
