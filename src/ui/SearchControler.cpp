#include "SearchControler.h"

SearchControler::SearchControler(Repository *repository, QObject *parent) :
    QObject(parent),
    model(new ApplicationListModel(this)),
    repository(repository)
{
    connect(repository, &Repository::updateStarted, this, &SearchControler::updateRepositoryStarted);
    connect(repository, &Repository::updateComplete, this, &SearchControler::handleUpdateRepositoryCompleted);
    connect(repository, &Repository::updateError, this, &SearchControler::updateRepositoryError);
}

void SearchControler::updateRepository() {
    repository->update();
}

void SearchControler::search(const QString &query) {
    this->query = query;
    filterApplications();
}

void SearchControler::handleUpdateRepositoryCompleted() {
    emit updateRepositoryCompleted();
    filterApplications();
}

void SearchControler::filterApplications() {
    emit searching();
    QList<Application> results = repository->filterLatestsVersions(query);
    model->setApplications(results);
    emit resultsReady();
}
