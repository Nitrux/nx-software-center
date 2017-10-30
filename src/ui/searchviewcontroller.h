#ifndef SEARCHVIEWCONTROLLER_H
#define SEARCHVIEWCONTROLLER_H

#include <QObject>

#include "../interactors/searchapplicationsinteractor.h"
#include "../interactors/searchapplicationsinteractorlistener.h"


class SearchViewController : public QObject, public SearchApplicationsInteractorListener
{
    Q_OBJECT
public:
    explicit SearchViewController(Registry *m_registry, QList<Repository *> repositories, QObject *parent = nullptr);

signals:
    void noMatchFound();
    void applications(QVariantList apps);

public slots:
    void search(QString searchString = QString());

protected:
    virtual void handleApplicationsList(const QVariantList &applicationsList) override;
    virtual void handleNoApplicationsFound() override;

    Registry *m_registry = nullptr;
    SearchApplicationsInteractor * interactor = nullptr;
    QList<Repository *> m_repositories;
    bool busy;
};

#endif // SEARCHVIEWCONTROLLER_H
