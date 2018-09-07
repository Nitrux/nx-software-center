#ifndef SEARCHCONTROLER_H
#define SEARCHCONTROLER_H

#include <QDebug>
#include <QObject>
#include "gateways/ApplicationRepositoryRestClient.h"
#include "ApplicationListModel.h"

class SearchController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ApplicationListModel *model MEMBER model NOTIFY modelChanged);
    Q_PROPERTY(bool isBusy MEMBER busy NOTIFY isBusyChanged);

    ApplicationRepository *repository;
    ApplicationsRepositorySearch *searchRequest;
    QString query;
    bool busy;
    ApplicationListModel *model;

    int page;
    int itemsPerPage;
public:
    explicit SearchController(ApplicationRepository* repository, QObject* parent = nullptr);

signals:
    void isBusyChanged(const bool &isBusy);
    void modelChanged(ApplicationListModel *model);
public slots:
    void search(const QString &query);
    void nextPage();
    void previousPage();

protected slots:
    void handleSearchResults();
protected:
    void doSearch();
};

#endif // SEARCHCONTROLER_H
