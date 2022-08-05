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
  enum Arch {
      all,
      amd64,
      arm64
  };

  Store(QObject *parent = nullptr) : QObject(parent) {}

  virtual const QString name() = 0;
  virtual void getCategories() = 0;
  virtual void getApplications(QList<QString> categoriesFilter,
                               QString nameFilter = "",
                               SORT_MODE sortMode = MODE_NEWEST,
                               QString page = "", QString pageSize = "",
                               QList<QString> tags = {}, QString user = "") = 0;
  virtual void getApplicationsByArch(QList<QString> categoriesFilter, QString nameFilter = "",
                             SORT_MODE sortMode = MODE_NEWEST, QString page = "",
                             QString pageSize = "", QList<QString> tags = {}, QString user = "",
                             Arch arch = Arch::all) = 0;

signals:
  void categoriesResponseReady(CategoryResponseDTO *response);
  void applicationsResponseReady(ApplicationResponseDTO *response);
  void error(QNetworkReply::NetworkError error);
};

#endif
