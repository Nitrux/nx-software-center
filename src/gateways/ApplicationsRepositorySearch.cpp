//
// Created by alexis on 7/10/18.
//

#include "ApplicationsRepositorySearch.h"

bool ApplicationsRepositorySearch::isRunning() const {
    return running;
}

void ApplicationsRepositorySearch::setQuery(const QString &query) {
    ApplicationsRepositorySearch::query = query;
}

void ApplicationsRepositorySearch::setCategory(const QString &category) {
    ApplicationsRepositorySearch::category = category;
}

void ApplicationsRepositorySearch::setOffset(const int &offset) {
    ApplicationsRepositorySearch::offset = offset;
}

void ApplicationsRepositorySearch::setLimit(const int &limit) {
    ApplicationsRepositorySearch::limit = limit;
}

QList<ApplicationAbstract> ApplicationsRepositorySearch::getResults() const {
    return results;
}

ApplicationsRepositorySearch::ApplicationsRepositorySearch(QObject *parent): QObject(parent), offset(0), limit(0) {}

ApplicationsRepositorySearch::~ApplicationsRepositorySearch()
{

}
