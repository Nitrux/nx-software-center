//
// Created by alexis on 25/01/18.
//

#include <QtCore/QByteArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include "ApplicationSerializer.h"
#include "Application.h"

QByteArray ApplicationSerializer::serialize(Application application) {
    QJsonObject object;
    object.insert(APP_ID, application.getId());
    object.insert(APP_VERSION, application.getVersion());
    object.insert(APP_CODE_NAME, application.getCodeName());
    object.insert(APP_NAME, application.getName());
    object.insert(APP_DESCRIPTION, application.getDescription());
    object.insert(APP_ARCH, application.getArch());
    object.insert(APP_DOWNLOAD_URL, application.getDownloadUrl());
    object.insert(APP_LICENCE, application.getLicence());
    object.insert(APP_HOME_PAGE, application.getHomePageUrl());

    object.insert(APP_AUTHORS, QJsonArray::fromStringList(application.getAuthors()));
    object.insert(APP_CATEGORIES, QJsonArray::fromStringList(application.getCategories()));
    object.insert(APP_SCREENSHOTS, QJsonArray::fromStringList(application.getScreenshots()));

    QJsonDocument document(object);
    QByteArray json = document.toJson(QJsonDocument::Compact);

    return json;
}

Application ApplicationSerializer::deserialize(QByteArray json) {
    QJsonObject object = QJsonDocument::fromJson(json).object();

    const QString &codeName = object.value(APP_CODE_NAME).toString();
    const QString &version = object.value(APP_VERSION).toString();

    Application application(codeName, version);
    application.setArch(object.value(APP_ARCH).toString());
    application.setDownloadUrl(object.value(APP_DOWNLOAD_URL).toString());
    application.setLicence(object.value(APP_LICENCE).toString());
    application.setName(object.value(APP_NAME).toString());
    application.setDescription(object.value(APP_DESCRIPTION).toString());
    application.setHomePageUrl(object.value(APP_HOME_PAGE).toString());

    const QStringList authorsList = getStringListFromJsonArray(object.value(APP_AUTHORS).toArray());
    application.setAuthors(authorsList);

    const QStringList categoriesList = getStringListFromJsonArray(object.value(APP_CATEGORIES).toArray());
    application.setCategories(categoriesList);

    const QStringList screenshotList = getStringListFromJsonArray(object.value(APP_SCREENSHOTS).toArray());
    application.setScreenshots(screenshotList);

    return application;
}

QStringList ApplicationSerializer::getStringListFromJsonArray(const QJsonArray &array) {
    QStringList list;
    for (const QJsonValue value: array)
        list.append(value.toString());
    return list;
}
