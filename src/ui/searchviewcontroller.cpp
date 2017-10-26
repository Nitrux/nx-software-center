#include "searchviewcontroller.h"

#include <QtConcurrent>

SearchViewController::SearchViewController(QList<Repository *> repositories, QObject *parent) : QObject(parent)
{
    m_repositories = repositories;
}

void SearchViewController::search(QString searchString)
{
    if (interactor == nullptr)
    {
        interactor = new SearchApplicationsInteractor(searchString, m_repositories, this);
        QtConcurrent::run([=]() {
            interactor->execute();
            interactor = nullptr;
        });
    }
}

void SearchViewController::handleApplicationsList(const QVariantList &applicationsList)
{
    QVariantList outputList;
    for (QVariant variant: applicationsList)
        outputList << QVariant::fromValue(variant.toMap());

    emit applications(outputList);
}

void SearchViewController::handleNoApplicationsFound()
{
    emit noMatchFound();
}
