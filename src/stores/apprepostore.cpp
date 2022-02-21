#include "apprepostore.h"

AppRepoStore::AppRepoStore(QString apiBaseUrl, QObject *parent) : QObject(parent) {
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
void AppRepoStore::getGroups(CategoryResponseDTO *appimagehubResponse) {
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	QUrlQuery query;
	query.addQueryItem("format", "json");
	QUrl url = QUrl::fromUserInput(QString(API_GROUPS_URL));
	url.setQuery(query);

	QNetworkReply *reply = manager->get(QNetworkRequest(url));

	connect(manager, &QNetworkAccessManager::finished, this, [this, appimagehubResponse](QNetworkReply *reply) {
		parseGetGroupsResponseAndReply(reply, appimagehubResponse);
	});

	connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError err) { emit error(err); });
}

/**
 * Fetch all packages from AppRepo API
 */
void AppRepoStore::getPackages() {
	this->getPackages(AppRepoStore::SearchPackage::ALL);
}

/**
 * Fetch packages by slug from AppRepo API
 */
void AppRepoStore::getPackagesBySlug(QString slug) {
	this->getPackages(AppRepoStore::SearchPackage::BY_SLUG, slug);
}

/**
 * Fetch packages by group from AppRepo API
 */
void AppRepoStore::getPackagesByGroup(int group) {
	this->getPackages(AppRepoStore::SearchPackage::BY_GROUP, QString::number(group));
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
    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError err) { emit error(err); });
}

void AppRepoStore::parseGetGroupsResponseAndReply(QNetworkReply *reply, CategoryResponseDTO *appimagehubResponse) {
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

	// This is done in order to display the apprepo categories as sub categories
	// of top level categories
	mapCategoryToSubcategory(appimagehubResponse, response);

    emit groupsResponseReady(appimagehubResponse);
}

void AppRepoStore::mapCategoryToSubcategory(CategoryResponseDTO *appimagehubResponse, QList<AppRepoGroupResponseDTO *> apprepoResponse) {

	foreach(AppRepoGroupResponseDTO *responseItem, apprepoResponse) {
		if ( audioCategories.contains(responseItem->getName()) ) {
			populateCategoryToSubcategory("Audio", appimagehubResponse, responseItem);
		} else if ( educationCategories.contains(responseItem->getName()) ) {
			populateCategoryToSubcategory("Education", appimagehubResponse, responseItem);
		} else if ( graphicsCategories.contains(responseItem->getName()) ) {
			populateCategoryToSubcategory("Graphics", appimagehubResponse, responseItem);
		} else if ( internetCategories.contains(responseItem->getName()) ) {
			populateCategoryToSubcategory("Internet", appimagehubResponse, responseItem);
		} else if ( officeCategories.contains(responseItem->getName()) ) {
			populateCategoryToSubcategory("Office", appimagehubResponse, responseItem);
		} else if ( programmingCategories.contains(responseItem->getName()) ) {
			populateCategoryToSubcategory("Programming", appimagehubResponse, responseItem);
		} else if ( systemAndToolsCategories.contains(responseItem->getName()) ) {
			populateCategoryToSubcategory("System & Tools", appimagehubResponse, responseItem);
		} else if ( videoCategories.contains(responseItem->getName()) ) {
			populateCategoryToSubcategory("Video", appimagehubResponse, responseItem);
		}
	}
}

void AppRepoStore::populateCategoryToSubcategory(QString categoryType, CategoryResponseDTO *appimagehubResponse, AppRepoGroupResponseDTO *responseItem) {
	foreach(Category *item, appimagehubResponse->categories) {
		if ( item->name == categoryType ) {
			Category *category = new Category();
			category->id = QString::number(responseItem->getUnique());
			category->name = "AppRepo: " + responseItem->getName();
			category->displayName = "AppRepo: " + responseItem->getName();
			category->categoryStore = Category::CategoryStore::APPREPO;

			item->categories.append(category);
		}
	}
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

    emit packagesResponseReady(generatePackageResponse(response));
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
	
	if ( obj.value("icon").toString() != "" ) {
		images << obj.value("icon").toString();
	}
	
	foreach(const QJsonValue &value, imagesArr) {
		QJsonObject obj = value.toObject();

		images << obj.value("url").toString();
	}
	packageItem->setImages(images);

	return packageItem;
}

ApplicationResponseDTO *AppRepoStore::generatePackageResponse(QList<AppRepoPackageResponseDTO *> response) {
	ApplicationResponseDTO *applicationResponse = new ApplicationResponseDTO();

	foreach(AppRepoPackageResponseDTO *responseItem, response) {
		PlingStoreApplicationData *application = new PlingStoreApplicationData();
		application->id = responseItem->getSlug();
		application->name = responseItem->getName();
		application->description = responseItem->getDescription();
		application->version = responseItem->getVersion();
		application->tags = "app,appimage,apprepo";
		application->applicationStore = Category::CategoryStore::APPREPO;

		foreach(AppRepoVersionDTO version, responseItem->getVersions()) {
			PlingStoreApplicationData::Download *download = new PlingStoreApplicationData::Download();
			download->name = version.getName();
			download->link = version.getFile().toString();

			application->downloads.append(download);
		}

		foreach(QString image, responseItem->getImages()) {
			PlingStoreApplicationData::PreviewPic *previewPic = new PlingStoreApplicationData::PreviewPic();
			previewPic->pic = image;
			previewPic->smallPic = image;

			application->previewPics.append(previewPic);
		}

		applicationResponse->applications.append(application);
	}

	return applicationResponse;
}
