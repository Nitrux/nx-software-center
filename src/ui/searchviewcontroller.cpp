#include "searchviewcontroller.h"

#include <QtConcurrent>

SearchViewController::SearchViewController(Registry *registry,
                                           QList<Repository *> repositories,
                                           QObject *parent)
    : QObject(parent) {
  m_registry = registry;
  m_repositories = repositories;
}

void SearchViewController::search(QString searchString) {
  qDebug() << "Searching";
  if (interactor == nullptr) {
    interactor = new SearchApplicationsInteractor(searchString, m_registry,
                                                  m_repositories, this);
    QtConcurrent::run([=]() {
      interactor->execute();
      interactor = nullptr;
    });
  }
}

void SearchViewController::handleApplicationsList(
    const QVariantList &applicationsList) {
  QVariantList outputList;
  for (QVariant variant : applicationsList)
    outputList << QVariant::fromValue(variant.toMap());

  delete interactor;
  interactor = nullptr;
  emit applications(outputList);
}

void SearchViewController::handleNoApplicationsFound() { emit noMatchFound(); }
