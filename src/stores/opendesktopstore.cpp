#include "opendesktopstore.h"
#include "ResponseDTO/applicationresponsedto.h"
#include "ResponseDTO/categoryresponsedto.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QUrlQuery>

OpenDesktopStore::OpenDesktopStore(QObject *parent, QString apiBaseUrl)
    : Store(parent) {
  API_BASEURL = apiBaseUrl;
  API_CATEGORIES_URL = QUrl(API_BASEURL + "/content/categories");
  API_LISTING_URL = QUrl(API_BASEURL + "/content/data");
}

const QString OpenDesktopStore::name() { return "Opendesktop"; }

void OpenDesktopStore::getCategories() {
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, &QNetworkAccessManager::finished, this,
          &OpenDesktopStore::parseGetCategoriesResponseAndReply);

  QUrlQuery query;
  query.addQueryItem("format", "json");
  QUrl url = API_CATEGORIES_URL;
  url.setQuery(query);

  QNetworkReply *reply = manager->get(QNetworkRequest(url));
  connect(
      reply, &QNetworkReply::errorOccurred,
      this, [=](QNetworkReply::NetworkError err) { emit error(err); });
}

void OpenDesktopStore::getApplications(QList<QString> categoriesFilter,
                                       QString nameFilter,
                                       Store::SORT_MODE sortMode, QString page,
                                       QString pageSize, QList<QString> tags) {
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, &QNetworkAccessManager::finished, this,
          &OpenDesktopStore::parseGetApplicationsResponseAndReply);

  QUrlQuery query;
  query.addQueryItem("format", "json");
  query.addQueryItem("categories", categoriesFilter.join("x"));
  query.addQueryItem("search", nameFilter);
  query.addQueryItem("tags", tags.join(","));

  switch (sortMode) {
  case MODE_NEWEST:
    query.addQueryItem("sortmode", "new");
    break;

  case MODE_ALPHABETICAL:
    query.addQueryItem("sortmode", "alpha");
    break;

  case MODE_HIGHEST_RATED:
    query.addQueryItem("sortmode", "high");
    break;

  case MODE_MOST_DOWNLOADED:
    query.addQueryItem("sortmode", "down");
    break;
  }

//  query.addQueryItem("packagetype", "appimage");
  query.addQueryItem("page", page);
  query.addQueryItem("pagesize", pageSize);

  QUrl url = API_LISTING_URL;
  url.setQuery(query);

  qDebug() << url;

  QNetworkReply *reply = manager->get(QNetworkRequest(url));
  connect(
      reply, &QNetworkReply::errorOccurred,
      this, [=](QNetworkReply::NetworkError err) { emit error(err); });
}

void OpenDesktopStore::getApplicationsByArch(QList<QString> categoriesFilter, QString nameFilter,
                                             SORT_MODE sortMode, QString page,
                                             QString pageSize, QList<QString> tags,
                                             Arch arch) {
    switch (arch) {
        case Arch::amd64 :
            tags.append("x86-64");
            break;

        case Arch::arm64 :
            tags.append("arm64");
            break;

        case Arch::all :
            break;
    }

    getApplications(categoriesFilter, nameFilter, sortMode, page, pageSize, tags);
}

void OpenDesktopStore::parseGetCategoriesResponseAndReply(
    QNetworkReply *reply) {
  CategoryResponseDTO *response = new CategoryResponseDTO(this);
  QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
  QJsonObject root = doc.object();

  response->message = root["message"].toString();
  response->status = root["status"].toString();
  response->statusCode = root["statuscode"].toDouble();
  response->totalItems = root["totalitems"].toDouble();

  QJsonArray array = root["data"].toArray();

  /**
   * @brief parentData
   *
   * {
   *    "id": {obj data},
   *    ...
   * }
   */
  QJsonObject parentJson;
  /**
   * @brief categoriesJson
   *
   * {
   *    "id": [array of children objs],
   *    ...
   * }
   */
  QJsonObject categoriesJson;

  for (QJsonValueRef item : array) {
    QJsonObject obj = item.toObject();

    if (obj["parent_id"].toString() == "") {
      categoriesJson.insert(obj["id"].toString(), QJsonArray());
      parentJson.insert(obj["id"].toString(), obj);
    } else {
      if (parentJson[obj["parent_id"].toString()].isNull()) {
        categoriesJson.insert(obj["parent_id"].toString(), QJsonArray());

        for (QJsonValueRef ref : array) {
          if (ref.toObject()["id"].toString() == obj["parent_id"].toString()) {
            parentJson.insert(obj["parent_id"].toString(), ref.toObject());
            break;
          }
        }
      } else {
        QJsonArray arr = categoriesJson[obj["parent_id"].toString()].toArray();
        arr.append(obj);

        categoriesJson[obj["parent_id"].toString()] = arr;
      }
    }
  }

  QList<Category *> list;

  for (const auto &key : categoriesJson.keys()) {
    QList<Category *> childList;
    QJsonArray arr = categoriesJson[key].toArray();

    for (QJsonValueRef ref : arr) {
      QJsonObject obj = ref.toObject();

      Category *c = new Category();
      c->id = obj["id"].toString();
      c->parentId = obj["parent_id"].toString();
      c->name = obj["name"].toString();
      c->displayName = obj["display_name"].toString();
      c->xdgType = obj["xdg_type"].toString();

      childList.append(c);
    }

    QJsonObject parentObj = parentJson[key].toObject();

    Category *c = new Category();
    c->id = parentObj["id"].toString();
    c->parentId = parentObj["parent_id"].toString();
    c->name = parentObj["name"].toString();
    c->displayName = parentObj["display_name"].toString();
    c->xdgType = parentObj["xdg_type"].toString();
    c->categories = childList;

    list.append(c);

    parentJson.remove(key);
    categoriesJson.remove(key);
  }

  /**
   * Will work only for 3 level categories
   *
   * TODO: Make the algo generic so that it works for any number of levels
   */
  for (int i = 0; i < list.count(); i++) {
    Category *c = list.at(i);

    if (c->parentId != "") {
      for (Category *cc : list) {
        if (cc->id == c->parentId) {
          cc->categories.append(c);
          list.removeAt(i);
          continue;
        }
      }
    }
  }

  response->categories = list;

  emit categoriesResponseReady(response);
}

void OpenDesktopStore::parseGetApplicationsResponseAndReply(
    QNetworkReply *reply) {
  QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
  ApplicationResponseDTO *response = new ApplicationResponseDTO(this);

  QJsonObject root = doc.object();

  response->message = root["message"].toString();
  response->status = root["status"].toString();
  response->statusCode = root["statuscode"].toDouble();
  response->totalItems = root["totalitems"].toString();
  response->itemsPerPage = root["itemsperpage"].toDouble();

  QJsonArray arr = root["data"].toArray();

  for (int i = 0; i < arr.size(); i++) {
    QJsonObject obj = arr[i].toObject();
    Application *app = new Application(this);

    app->id = obj["id"].toString();
    app->name = obj["name"].toString();
    app->tags = obj["tags"].toString();
    app->score = obj["score"].toDouble();
    app->typeId = obj["typeid"].toString();
    app->changed = obj["changed"].toString();
    app->created = obj["created"].toString();
    app->details = obj["details"].toString();
    app->summary = obj["summary"].toString();
    app->version = obj["version"].toString();
    app->xdgType = obj["xdg_type"].toString();
    app->comments = obj["comments"].toString();
    app->language = obj["language"].toString();
    app->personId = obj["personid"].toString();
    app->typeName = obj["typename"].toString();
    app->totalDownloads = obj["downloads"].toDouble();
    app->detailPage = obj["detailpage"].toString();
    app->description = obj["description"].toString();
    app->ghnsExcluded = obj["ghns_excluded"].toString();

    obj.remove("id");
    obj.remove("name");
    obj.remove("tags");
    obj.remove("score");
    obj.remove("typeid");
    obj.remove("changed");
    obj.remove("created");
    obj.remove("details");
    obj.remove("summary");
    obj.remove("version");
    obj.remove("xdg_type");
    obj.remove("comments");
    obj.remove("language");
    obj.remove("personid");
    obj.remove("typename");
    obj.remove("downloads");
    obj.remove("detailpage");
    obj.remove("description");
    obj.remove("ghns_excluded");

    /**
     * @brief parsedJson
     *
     * {
     *      downloads: { <index>: {<data>}, ... },
     *      previewPics: { <index>: {<data>}, ... },
     *      previewUrls: { <index>: {<data>}, ... }
     * }
     */
    QJsonObject parsedJson;
    parsedJson["downloads"] = QJsonObject();
    parsedJson["previewPics"] = QJsonObject();
    parsedJson["previewUrls"] = QJsonObject();

    for (const QString &key : obj.keys()) {
      QRegularExpression re(
          "(download|previewpic|smallpreviewpic|preview)_*(\\w*)(\\d+)");
      QRegularExpressionMatch match = re.match(key);
      QString type = match.captured(1);

      if (type == "download") {
        QJsonObject o = parsedJson["downloads"].toObject();

        if (parsedJson["downloads"]
                .toObject()[match.captured(3)]
                .isUndefined()) {
          o[match.captured(3)] = QJsonObject();
        }

        QJsonObject p = o[match.captured(3)].toObject();
        p[match.captured(2)] = obj[key];
        o[match.captured(3)] = p;
        parsedJson["downloads"] = o;
      } else if (type == "previewpic") {
        QJsonObject o = parsedJson["previewPics"].toObject();

        if (parsedJson["previewPics"]
                .toObject()[match.captured(3)]
                .isUndefined()) {
          o[match.captured(3)] = QJsonObject();
        }

        QJsonObject p = o[match.captured(3)].toObject();
        p[match.captured(1)] = obj[key];
        o[match.captured(3)] = p;
        parsedJson["previewPics"] = o;
      } else if (type == "smallpreviewpic") {
        QJsonObject o = parsedJson["previewPics"].toObject();

        if (parsedJson["previewPics"]
                .toObject()[match.captured(3)]
                .isUndefined()) {
          o[match.captured(3)] = QJsonObject();
        }

        QJsonObject p = o[match.captured(3)].toObject();
        p[match.captured(1)] = obj[key];
        o[match.captured(3)] = p;
        parsedJson["previewPics"] = o;
      } else if (type == "preview") {
        QJsonObject o = parsedJson["previewUrls"].toObject();

        if (parsedJson["previewUrls"]
                .toObject()[match.captured(3)]
                .isUndefined()) {
          o[match.captured(3)] = QJsonObject();
        }

        QJsonObject p = o[match.captured(3)].toObject();
        p[match.captured(1)] = obj[key];
        o[match.captured(3)] = p;
        parsedJson["previewUrls"] = o;
      }
    }

    QList<Application::Download *> downloads;

    for (QString key : parsedJson["downloads"].toObject().keys()) {
      Application::Download *download = new Application::Download();

      download->name =
          parsedJson["downloads"].toObject()[key].toObject()["name"].toString();
      download->link =
          parsedJson["downloads"].toObject()[key].toObject()["link"].toString();
      download->way =
          parsedJson["downloads"].toObject()[key].toObject()["way"].toString();
      download->size =
          QString::number((qint64)parsedJson["downloads"].toObject()[key].toObject()["size"].toDouble());
      download->tags =
          parsedJson["downloads"].toObject()[key].toObject()["tags"].toString();
      download->type =
          parsedJson["downloads"].toObject()[key].toObject()["type"].toString();
      download->price = parsedJson["downloads"]
                            .toObject()[key]
                            .toObject()["price"]
                            .toString();
      download->repository = parsedJson["downloads"]
                                 .toObject()[key]
                                 .toObject()["repository"]
                                 .toString();
      download->packageArch = parsedJson["downloads"]
                                  .toObject()[key]
                                  .toObject()["package_arch"]
                                  .toString();
      download->packageName = parsedJson["downloads"]
                                  .toObject()[key]
                                  .toObject()["packagename"]
                                  .toString();
      download->packageType = parsedJson["downloads"]
                                  .toObject()[key]
                                  .toObject()["package_type"]
                                  .toString();
      download->gpgSignature = parsedJson["downloads"]
                                   .toObject()[key]
                                   .toObject()["gpgsignature"]
                                   .toString();
      download->gpgFingerprint = parsedJson["downloads"]
                                     .toObject()[key]
                                     .toObject()["gpgfingerprint"]
                                     .toString();

      downloads.append(download);
    }

    QList<Application::PreviewPic *> previewPics;

    for (QString key : parsedJson["previewPics"].toObject().keys()) {
      Application::PreviewPic *previewPic = new Application::PreviewPic();

      previewPic->pic = parsedJson["previewPics"]
                            .toObject()[key]
                            .toObject()["previewpic"]
                            .toString();

      previewPic->smallPic = parsedJson["previewPics"]
                                 .toObject()[key]
                                 .toObject()["smallpreviewpic"]
                                 .toString();

      previewPics.append(previewPic);
    }

    QList<Application::PreviewUrl *> previewUrls;

    for (QString key : parsedJson["previewUrls"].toObject().keys()) {
      Application::PreviewUrl *previewUrl = new Application::PreviewUrl();

      previewUrl->preview = parsedJson["previewUrls"]
                                .toObject()[key]
                                .toObject()["preview"]
                                .toString();

      previewUrls.append(previewUrl);
    }

    app->downloads = downloads;
    app->previewPics = previewPics;
    app->previewUrls = previewUrls;
    response->applications.append(app);
  }

  emit applicationsResponseReady(response);
}
