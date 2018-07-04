#include "SearchController.h"

SearchController::SearchController(RestClient* explorer, QObject* parent)
        :
        QObject(parent),
        model(new ApplicationListModel(this)), explorer(explorer)
{
    filterApplications();
}

void SearchController::search(const QString &query) {
    this->query = query;
    filterApplications();
}

void SearchController::handleRepositoryChanged()
{
    filterApplications();
}

void SearchController::filterApplications() {
    emit searching();
    connect(explorer, &RestClient::searchCompleted, this, &SearchController::handleSearchCompleted);
    connect(explorer, &RestClient::failure, this, &SearchController::failed);
    explorer->search(query);

}
void SearchController::handleSearchCompleted(const QList<QVariantMap> applications)
{
    disconnect(explorer, &RestClient::searchCompleted, this, &SearchController::handleSearchCompleted);
    disconnect(explorer, &RestClient::failure, this, &SearchController::failed);
    model->setApplications(applications);
    emit resultsReady();
}
