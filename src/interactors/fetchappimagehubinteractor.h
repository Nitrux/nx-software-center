#ifndef FETCHAPPIMAGEHUBINTERACTOR_H
#define FETCHAPPIMAGEHUBINTERACTOR_H

#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>

#include "interactor.h"

class App;
class Repository;
class FetchAppImageHubInteractor : public Interactor {
  Q_OBJECT

public:
  explicit FetchAppImageHubInteractor(const QString &url,
                                      Repository *repository,
                                      QObject *parent = nullptr);

  virtual ~FetchAppImageHubInteractor();
public slots:
  virtual void execute();

protected slots:
  void handleNetworkReply();
  void findDownloadLinks(App *app, QString arch);

private:
  QUrl dataUrl;
  Repository *repository;

  QJsonArray data;
  QNetworkAccessManager networkAccessManager;
  QNetworkReply *networkReply = nullptr;
  int requestsActive = 0;
};

#endif // FETCHAPPIMAGEHUBINTERACTOR_H
