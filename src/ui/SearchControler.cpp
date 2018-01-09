#include "SearchControler.h"

SearchControler::SearchControler(QObject *parent) :
    QObject(parent),
    model(new ApplicationListModel(this)),
    repository(parent),
    i(nullptr)
{

}

void SearchControler::fetch() {
    if (i == nullptr) {
        emit searching();
        i = new FetchApplicationsInteractor(sources, this);
        connect(i, &FetchApplicationsInteractor::completed, this, &SearchControler::handleFetchApplicationsCompleted);
        // i->moveToThread();
        i->execute();
    }
}

void SearchControler::search(const QString &query) {
    this->query = query;
    filterApplications();
}

void SearchControler::handleFetchApplicationsCompleted(QList<Application> results, QStringList errors) {
    if (results.isEmpty() && !errors.isEmpty())
        emit fetchError();

    repository.removeAll();
    for (const Application &a: results)
        repository.add(a);

    emit fetchCompleted();

    filterApplications();

    for (QString error: errors)
        qWarning() << error;
}

void SearchControler::filterApplications() {
    emit searching();
    QList<Application> results = repository.filterLatestsVersions(query);
    model->setApplications(results);
    emit resultsReady();
}
