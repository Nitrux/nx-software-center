#include "fetchappimagehubinteractor.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "../entities/app.h"
#include "../entities/release.h"
#include "../entities/repository.h"

FetchAppImageHubInteractor::FetchAppImageHubInteractor(const QString &url,
                                                       Repository *repository,
                                                       QObject *parent)
    : Interactor(parent) {
  Q_ASSERT(repository != nullptr);
  this->dataUrl = QUrl(url);
  this->repository = repository;
}

FetchAppImageHubInteractor::~FetchAppImageHubInteractor() {}

void FetchAppImageHubInteractor::execute() {
  requestsActive++;
  auto request = QNetworkRequest(dataUrl);
  request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

  networkReply = networkAccessManager.get(request);
  QObject::connect(networkReply, &QNetworkReply::finished, this,
                   &FetchAppImageHubInteractor::handleNetworkReply);
}

void FetchAppImageHubInteractor::handleNetworkReply() {
  if (networkReply == nullptr)
    return;

  qDebug() << "Reply"
           << networkReply->header(QNetworkRequest::ContentLengthHeader)
           << networkReply->header(QNetworkRequest::ContentTypeHeader);

  if (networkReply->error() == QNetworkReply::NoError) {
    QByteArray rawData = networkReply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    auto jsonRoot = doc.object();
    if (jsonRoot.contains("items")) {
      QList<App *> newItems;
      //                qDebug() << "Loading appimage items";
      data = jsonRoot.value("items").toArray();
      for (QJsonValue appImageData : data) {
        QJsonObject appImageDataObject = appImageData.toObject();

        App *appImage = new App(appImageDataObject.value("name").toString());
        Release *release = new Release("latest", appImage->id);
        release->name = appImageDataObject.value("name").toString();
        release->description =
            appImageDataObject.value("description").toString();
        release->licence = appImageDataObject.value("licence").toString();

        QStringList authors;
        for (QJsonValue value : appImageDataObject.value("autors").toArray())
          authors << value.toString();
        release->authors = authors;

        QStringList categories;
        for (QJsonValue value :
             appImageDataObject.value("categotries").toArray())
          categories << value.toString();
        appImage->categories = categories;

        QStringList screenshots;
        for (QJsonValue value :
             appImageDataObject.value("screenshots").toArray())
          screenshots << value.toString();
        release->screenshot_links = screenshots;

        QMap<QString, QString> links;
        auto linksArray = appImageDataObject.value("links").toArray();
        for (QJsonValue linkItem : linksArray) {
          auto linkObject = linkItem.toObject();
          auto key = linkObject.value("type").toString().toLower();
          auto value = linkObject.value("url").toString();

          links.insert(key, value);
        }
        appImage->links = links;

        appImage->addRelease(release);

        findDownloadLinks(appImage, QSysInfo::currentCpuArchitecture());

        repository->add(appImage);
      }

      //                qDebug() << newItems.count() << " appimage items
      //                loaded";
      requestsActive--;
      if (requestsActive == 0)
        emit complete();
    } else {
      qWarning() << "No \"items\" array found in reply";
    }
  } else {
    qDebug() << "Something went wrong: " << networkReply->errorString();
  }

  networkReply->deleteLater();
  networkReply = nullptr;
}

/**
 * This is a best effort find download links function it's supposed to work
 * for github releases but not for other sites.
 * It works asynchronously when finished the 'download' at the appImage will
 * be updated.
 **/
void FetchAppImageHubInteractor::findDownloadLinks(App *app, QString arch) {
  Q_ASSERT(app != nullptr);

  if (app->links.contains("install")) {
    QString url = app->links.value("install");
    auto request = QNetworkRequest(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    requestsActive++;
    auto reply = networkAccessManager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
      if (reply->error() == QNetworkReply::NoError) {
        QString rxString = "href=\"([^\"]*%1[^\"]*AppImage)\"";
        rxString = rxString.arg(arch);
        QRegExp rx(rxString);
        QString str = reply->readAll();
        QStringList list;
        int pos = 0;

        while ((pos = rx.indexIn(str, pos)) != -1) {
          QString capUrl = rx.cap(1);
          if (!capUrl.startsWith("http")) {
            QUrl url = reply->url();
            url.setPath(capUrl);
            capUrl = url.toString();
          }

          list << capUrl;
          pos += rx.matchedLength();

          rx.capturedTexts();
          break;
        }

        if (list.count() > 0) {
          auto release = app->lastRelease();
          release->download_link = list.first();
        }
      } else {
        qDebug() << reply->errorString()
                 << ": while resolving download link for " << app->id;
      }

      requestsActive--;
      if (requestsActive == 0)
        emit complete();
    });
  } else {
    qDebug() << app->id << " has no install link ";
  }
}
