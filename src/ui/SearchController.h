#ifndef SEARCHCONTROLER_H
#define SEARCHCONTROLER_H

#include <QDebug>
#include <QObject>
#include "entities/RestClient.h"
#include "ApplicationListModel.h"
#include "interactors/FetchApplicationsInteractor.h"

class SearchController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ApplicationListModel *model MEMBER model NOTIFY modelChanged);

    ApplicationListModel *model;
    RestClient *explorer;
    QString query;
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
    void handleRepositoryChanged();
    void handleSearchCompleted(const QList<QVariantMap> applications);
protected:
    void filterApplications();
};

#endif // SEARCHCONTROLER_H
