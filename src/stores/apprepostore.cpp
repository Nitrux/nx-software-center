#include "apprepostore.h"

AppRepoStore::AppRepoStore(QString apiBaseUrl) {
	API_BASEURL = apiBaseUrl;
	API_GROUPS_URL = API_BASEURL + "/package/groups/";
	API_PACKAGES_URL = API_BASEURL + "/package/";
	API_PACKAGES_BY_SLUG_URL = API_BASEURL + "/package/%1/by_slug/";
	API_PACKAGES_BY_GROUP_URL = API_BASEURL + "/package/%1/by_group/";
}

const QString AppRepoStore::name() { return "AppRepo"; }

/**
 * Fetch categories (groups) from AppRepo API
 */
void AppRepoStore::getGroups() {
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	QUrlQuery query;
	query.addQueryItem("format", "json");
	QUrl url = QUrl::fromUserInput(QString(API_GROUPS_URL));
	url.setQuery(query);

	QNetworkReply *reply = manager->get(QNetworkRequest(url));

	connect(manager, &QNetworkAccessManager::finished, this, &AppRepoStore::parseGetGroupsResponseAndReply);
	connect(reply, &QNetworkReply::errorOccurred, this, [=](QNetworkReply::NetworkError err) { emit error(err); });
}

void AppRepoStore::getPackages(SearchPackage criteria, QString value) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	QUrl url;
    QUrlQuery query;
    query.addQueryItem("format", "json");
    
	switch (criteria) {
		case ALL:
			url = QUrl::fromUserInput(QString(API_PACKAGES_URL));
			break;

		case BY_SLUG:
			url = QUrl::fromUserInput(QString(API_PACKAGES_BY_SLUG_URL).arg(value));
			break;

		case BY_GROUP:
			url = QUrl::fromUserInput(QString(API_PACKAGES_BY_GROUP_URL).arg(value));
			break;
	}

    url.setQuery(query);

	qDebug() << "Get package from App Repo: " << url;

    QNetworkReply *reply = manager->get(QNetworkRequest(url));

    connect(manager, &QNetworkAccessManager::finished, this, &AppRepoStore::parseGetPackagesResponseAndReply);
    connect(reply, &QNetworkReply::errorOccurred, this, [=](QNetworkReply::NetworkError err) { emit error(err); });
}

void AppRepoStore::parseGetGroupsResponseAndReply(QNetworkReply *reply) {
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray root = doc.array();

    QList<AppRepoGroupResponseDTO *> response;

    foreach (const QJsonValue &value, root) {
		QJsonObject obj = value.toObject();

		AppRepoGroupResponseDTO *categoryItem = new AppRepoGroupResponseDTO();
		categoryItem->setName(obj.value("name").toString());
		categoryItem->setDescription(obj.value("description").toString());
		categoryItem->setIcon(QUrl::fromUserInput(obj.value("icon").toString()));
		categoryItem->setUnique(obj.value("unique").toInt());

		response << categoryItem;
    }

    emit groupsResponseReady(response);
}

void AppRepoStore::parseGetPackagesResponseAndReply(QNetworkReply *reply) {
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
	
	QList<AppRepoPackageResponseDTO *> response;
    
	if ( doc.isArray() ) {
		QJsonArray root = doc.array();
		
		foreach (const QJsonValue &value, root) {
			QJsonObject obj = value.toObject();

			response << createPackageResponseDTO(obj);
		}
	} else if ( doc.isObject() ) {
		QJsonObject obj = doc.object();

		response << createPackageResponseDTO(obj);
	}

    emit packagesResponseReady(response);
}

AppRepoPackageResponseDTO *AppRepoStore::createPackageResponseDTO(QJsonObject obj) {
	AppRepoPackageResponseDTO *packageItem = new AppRepoPackageResponseDTO();
	packageItem->setName(obj.value("name").toString());
	packageItem->setSlug(obj.value("slug").toString());
	packageItem->setVersion(obj.value("version").toString());
	packageItem->setDescription(obj.value("description").toString());
	packageItem->setHash(obj.value("hash").toString());
	packageItem->setPackage(obj.value("package").toString());
	packageItem->setIcon(QUrl::fromUserInput(obj.value("icon").toString()));
	packageItem->setPreview(QUrl::fromUserInput(obj.value("preview").toString()));
	packageItem->setFile(QUrl::fromUserInput(obj.value("file").toString()));

	// versions
	QList<AppRepoVersionDTO> versions;
	QJsonArray versionsArr = obj.value("versions").toArray();
	foreach(const QJsonValue &value, versionsArr) {
		QJsonObject obj = value.toObject();

		AppRepoVersionDTO item;
		item.setName(obj.value("name").toString());
		item.setDescription(obj.value("description").toString());
		item.setHash(obj.value("hash").toString());
		item.setFile(QUrl::fromUserInput(obj.value("file").toString()));
		item.setIpfsCid(obj.value("ipfs_cid").toString());
		item.setIpfsGateway(obj.value("ipfs_gateway").toString());

		versions << item;
	}
	packageItem->setVersions(versions);

	// groups
	QList<AppRepoGroupResponseDTO> groups;
	QJsonArray groupsArr = obj.value("groups").toArray();
	foreach(const QJsonValue &value, groupsArr) {
		QJsonObject obj = value.toObject();

		AppRepoGroupResponseDTO item;
		item.setName(obj.value("name").toString());
		item.setDescription(obj.value("description").toString());
		item.setIcon(QUrl::fromUserInput(obj.value("icon").toString()));
		item.setUnique(obj.value("unique").toInt());

		groups << item;
	}
	packageItem->setGroups(groups);

	// images
	QStringList images;
	QJsonArray imagesArr = obj.value("images").toArray();
	foreach(const QJsonValue &value, imagesArr) {
		QJsonObject obj = value.toObject();

		images << obj.value("url").toString();
	}
	packageItem->setImages(images);

	return packageItem;
}