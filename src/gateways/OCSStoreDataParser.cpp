//
// Created by alexis on 3/6/18.
//
#include <QDebug>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QXmlResultItems>

#include "OCSStoreDataParser.h"


OCSStoreDataParser::OCSStoreDataParser(QObject *parent) : QObject(parent), itemsPerPage(0), totalItems(0),
                                                          failed(false) {
}

void OCSStoreDataParser::extractApplications() {
    int contentItems = countContentItems();

    if (!failed) {
        totalItems = evaluateQuery("/ocs/meta/totalitems/text()").toInt();
        itemsPerPage = evaluateQuery("/ocs/meta/itemsperpage/text()").toInt();

        for (int i = 1; i <= contentItems; i++)
            parseContentTag(i);

        emit completed();
    }
}

int OCSStoreDataParser::countContentItems() {
    QString result;
    query.setQuery("/ocs/data/count(content)");
    if (query.isValid())
        query.evaluateTo(&result);
    else {
        emit error(QString("Unable to open: %1").arg(target.toString()));
        failed = true;
    }

    return result.toInt();
}

void OCSStoreDataParser::parseContentTag(int idx) {
    QString name = getContentStringField(idx, "name");
    name = name.replace("-", " ").trimmed();

    QString codeName = name;
    codeName.replace(" ","-").toLower();

    QString version = getContentStringField(idx, "version");

    QString description = getContentStringField(idx, "description");
    QString autor = getContentStringField(idx, "personid");
    QString icon = getContentStringField(idx, "smallpreviewpic1");

    int appimageDownloadIdx = getAppimageDownloadIdx(idx);
    QString downloadLink = getContentStringField(idx, QString("downloadlink%1").arg(appimageDownloadIdx));
    QString downloadSizeStr = getContentStringField(idx, QString("downloadsize%1").arg(appimageDownloadIdx));

    QStringList screenShots = getScreenShots(idx);

    Application application(codeName, version);
    application.setName(name);
    application.setDescription(description);
    application.setAuthors({autor});
    application.setIcon(icon);
    application.setDownloadUrl(downloadLink);
    application.setDownloadSize(downloadSizeStr.toInt());
    application.setScreenshots(screenShots);

    if (!application.isEmpty() && !application.getDownloadUrl().isEmpty())
        results << application;
}

QStringList OCSStoreDataParser::getScreenShots(int contentIdx) {
    QString text;
    QString queryStr = "/ocs/data/content[%1]/*[matches(name(), '^previewpic')]/text()";
    query.setQuery(queryStr.arg(contentIdx));

    if (query.isValid())
        query.evaluateTo(&text);
    else
        qWarning() << "Invalid name query.";

    QStringList screenShots;
    for (QString screenShot: text.split(" ")) {
        screenShot = screenShot.trimmed();
        if (!screenShot.isEmpty())
            screenShots << screenShot;
    }

    return screenShots;
}

int OCSStoreDataParser::getAppimageDownloadIdx(int contentIdx) {
    int appimageDownloadIdx;
    QString text;
    QString queryStr = "/ocs/data/content[%1]/*[matches(text(), '.*\\.AppImage') and matches(name(), 'downloadlink.*')]/name()";
    query.setQuery(queryStr.arg(contentIdx));

    if (query.isValid())
        query.evaluateTo(&text);
    else
        qWarning() << "Invalid name query.";

    text = text.replace("downloadlink", "").trimmed();
    appimageDownloadIdx = text.toInt();
    return appimageDownloadIdx;
}

QString OCSStoreDataParser::getContentStringField(int idx, QString field) {
    auto queryStr = QString("/ocs/data/content[%1]/%2/text()").arg(idx).arg(field);
    QString name = evaluateQuery(queryStr);
    return name;
}

QString OCSStoreDataParser::evaluateQuery(QString queryString) {
    QString text;
    query.setQuery(queryString);

    if (query.isValid())
        query.evaluateTo(&text);
    else
        qWarning() << "Invalid name query.";
    return text.trimmed();
}

const QList<Application> &OCSStoreDataParser::getResults() const {
    return results;
}

int OCSStoreDataParser::getItemsPerPage() const {
    return itemsPerPage;
}

int OCSStoreDataParser::getTotalItems() const {
    return totalItems;
}

const QUrl &OCSStoreDataParser::getTarget() const {
    return target;
}

void OCSStoreDataParser::setTarget(const QUrl &target) {
    OCSStoreDataParser::target = target;

    query.setFocus(QUrl(OCSStoreDataParser::target));
}

bool OCSStoreDataParser::isFailed() const {
    return failed;
}
