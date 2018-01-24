#include "SearchControler.h"

SearchControler::SearchControler(Repository *repository, QObject *parent) :
    QObject(parent),
    model(new ApplicationListModel(this)),
    repository(repository)
{
    connect(this->repository, &Repository::changed, this, &SearchControler::handleRepositoryChanged);
    filterApplications();
}

void SearchControler::search(const QString &query) {
    this->query = query;
    filterApplications();
}

void SearchControler::handleRepositoryChanged()
{
    filterApplications();
}

void SearchControler::filterApplications() {
    emit searching();
    QList<Application> results = repository->filterLatestsVersions(query);
    model->setApplications(results);
    emit resultsReady();
}
