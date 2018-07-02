#include "SearchController.h"

SearchController::SearchController(Explorer* explorer, QObject* parent)
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
    connect(explorer, &Explorer::searchCompleted, this, &SearchController::handleSearchCompleted);
    explorer->search(query);

}
void SearchController::handleSearchCompleted(const QList<QVariantMap> applications)
{
    model->setApplications(applications);
    emit resultsReady();
}
