#ifndef STORES_STORE_H
#define STORES_STORE_H

#include "ResponseDTO/applicationresponsedto.h"
#include "ResponseDTO/category.h"
#include "ResponseDTO/categoryresponsedto.h"

#include <QJsonObject>
#include <QList>
#include <QNetworkReply>
#include <QObject>
#include <QString>

class Store : public QObject {
  Q_OBJECT
public:
  enum SORT_MODE {
    MODE_NEWEST,
    MODE_ALPHABETICAL,
    MODE_HIGHEST_RATED,
    MODE_MOST_DOWNLOADED
  };

  Store(QObject *parent = nullptr) : QObject(parent) {}

  virtual const QString name() = 0;
  virtual void getCategories() = 0;
  virtual void getApplications(QList<QString> categoriesFilter,
                               QString nameFilter = "",
                               SORT_MODE sortMode = MODE_NEWEST,
                               QString page = "", QString pageSize = "") = 0;

signals:
  void categorisResponseReady(CategoryResponseDTO *response);
  void applicationsResponseReady(ApplicationResponseDTO *response);
  void error(QNetworkReply::NetworkError error);
};

#endif
