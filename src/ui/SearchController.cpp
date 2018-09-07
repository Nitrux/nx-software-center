#include <gateways/ApplicationsSearchRequest.h>
#include "SearchController.h"


void SearchController::search(const QString &query) {
    this->query = query;
    page = 0;
    doSearch();
}

void SearchController::nextPage()
{
    page ++;
    doSearch();
}

void SearchController::previousPage()
{
    if (page > 0) {
        page --;
        doSearch();
    }
}

void SearchController::doSearch() {
    if (searchRequest != nullptr) {
        disconnect(searchRequest, nullptr, this, nullptr);
        searchRequest->stop();
        searchRequest->deleteLater();
        searchRequest = nullptr;
    }

    busy = true;
    emit isBusyChanged(busy);

    searchRequest = repository->buildSearchRequest(query, "");
    connect(searchRequest, &ApplicationsSearchRequest::completed, this, &SearchController::handleSearchResults);
    connect(searchRequest, &ApplicationsSearchRequest::failed, this, &SearchController::handleSearchResults);
    searchRequest->setLimit(itemsPerPage);
    searchRequest->setOffset(page * itemsPerPage);
    searchRequest->start();
}

void SearchController::handleSearchResults() {
    disconnect(searchRequest, &ApplicationsSearchRequest::completed, this, &SearchController::handleSearchResults);
    disconnect(searchRequest, &ApplicationsSearchRequest::failed, this, &SearchController::handleSearchResults);

    model->setApplications(searchRequest->getResults());

    searchRequest->deleteLater();
    searchRequest = nullptr;

    busy = false;
    emit isBusyChanged(busy);
}

SearchController::SearchController(ApplicationRepository *repository, QObject *parent) :
        QObject(parent), repository(repository), searchRequest(nullptr), model(new ApplicationListModel(this)),
        page(0), itemsPerPage(28) {
    doSearch();
}
