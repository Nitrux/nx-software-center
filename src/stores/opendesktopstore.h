#ifndef STORES_OPENDESKTOPSTORE_H
#define STORES_OPENDESKTOPSTORE_H

#include "ResponseDTO/category.h"
#include "store.h"

#include <QList>
#include <QNetworkReply>
#include <QString>
#include <QUrl>

class OpenDesktopStore : public Store {
  Q_OBJECT

public:
  OpenDesktopStore(QObject *parent, QString apiBaseUrl);

  const QString name() override;
  void getCategories() override final;
  void getApplications(QList<QString> categoriesFilter, QString nameFilter = "",
                       SORT_MODE sortMode = MODE_NEWEST, QString page = "",
                       QString pageSize = "", QList<QString> tags = {}, QString user = "") override final;
  void getApplicationsByArch(QList<QString> categoriesFilter, QString nameFilter = "",
                             SORT_MODE sortMode = MODE_NEWEST, QString page = "",
                             QString pageSize = "", QList<QString> tags = {}, QString user = "",
                             Arch arch = all) override final;

private:
  QString API_BASEURL;
  QUrl API_CATEGORIES_URL;
  QUrl API_LISTING_URL;

  void parseGetCategoriesResponseAndReply(QNetworkReply *reply);
  void parseGetApplicationsResponseAndReply(QNetworkReply *reply);
};

#endif
