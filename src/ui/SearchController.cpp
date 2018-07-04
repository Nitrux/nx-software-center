#include <gateways/ApplicationsSearchRequest.h>
#include "SearchController.h"

SearchController::SearchController(RestClient* explorer, QObject* parent)
        :
        QObject(parent),
        model(new ApplicationListModel(this)), restClient(explorer),
        searchRequest
                (nullptr)
{
    doSearch();
}

void SearchController::search(const QString& query)
{
    this->query = query;
    doSearch();
}

void SearchController::doSearch()
{
    emit searching();

    searchRequest = restClient->buildSearchRequest(query);
    connect(searchRequest, &ApplicationsSearchRequest::resultsReady, this, &SearchController::handleSearchResults);
    connect(searchRequest, &ApplicationsSearchRequest::failed, this, &SearchController::failed);
    searchRequest->start();
}
void SearchController::handleSearchResults()
{
    disconnect(searchRequest, &ApplicationsSearchRequest::resultsReady, this, &SearchController::handleSearchResults);
    disconnect(searchRequest, &ApplicationsSearchRequest::failed, this, &SearchController::failed);

    model->setApplications(searchRequest->getResults());

    searchRequest->deleteLater();
    searchRequest = nullptr;

    emit resultsReady();
}
