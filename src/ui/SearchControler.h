#ifndef SEARCHCONTROLER_H
#define SEARCHCONTROLER_H

#include <QDebug>
#include <QObject>
#include "entities/Application.h"
#include "entities/Repository.h"
#include "ApplicationListModel.h"
#include "interactors/FetchApplicationsInteractor.h"

class SearchControler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ApplicationListModel *model MEMBER model NOTIFY modelChanged);

    ApplicationListModel *model;
    Repository repository;
    QList<Source*> sources;
    FetchApplicationsInteractor *i;
    QString query;
public:
    explicit SearchControler(QObject *parent = nullptr);
    void setSources(const QList<Source*> &sources) {this->sources = sources;}

signals:
    void fetchingApplications();
    void fetchCompleted();
    void fetchError();

    void searching();
    void resultsReady();

    void modelChanged(ApplicationListModel *model);
public slots:
    void fetch();

    void search(const QString &query);

protected slots:
    void handleFetchApplicationsCompleted(QList<Application> results, QStringList errors);

protected:
    void filterApplications();
};

#endif // SEARCHCONTROLER_H
