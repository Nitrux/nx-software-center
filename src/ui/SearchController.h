#ifndef SEARCHCONTROLER_H
#define SEARCHCONTROLER_H

#include <QDebug>
#include <QObject>
#include "gateways/RestClient.h"
#include "ApplicationListModel.h"
#include "interactors/FetchApplicationsInteractor.h"

class ApplicationSearchRequest;
class SearchController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ApplicationListModel *model MEMBER model NOTIFY modelChanged);

    ApplicationListModel *model;
    RestClient *restClient;
    QString query;
    ApplicationsSearchRequest *searchRequest;
public:
    explicit SearchController(RestClient* explorer, QObject* parent = nullptr);

signals:
    void searching();
    void resultsReady();
    void failed();

    void modelChanged(ApplicationListModel *model);
public slots:
    void search(const QString &query);

protected slots:
    void handleSearchResults();
protected:
    void doSearch();
};

#endif // SEARCHCONTROLER_H
