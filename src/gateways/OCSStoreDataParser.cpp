//
// Created by alexis on 3/6/18.
//
#include <QDebug>
#include <QString>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QXmlResultItems>

#include "OCSStoreDataParser.h"


OCSStoreDataParser::OCSStoreDataParser(const QUrl &target, QObject *parent) : QObject(parent), target(target),
                                                                              itemsPerPage(0), totalItems(0) {}

void OCSStoreDataParser::extractApplications() {
    int contentItems = countContentItems();

    totalItems = evaluateQuery("/ocs/meta/totalitems/text()").toInt();
    itemsPerPage = evaluateQuery("/ocs/meta/itemsperpage/text()").toInt();

    for (int i = 1; i <= contentItems; i++)
        parseContentTag(i);

    emit completed();
}

int OCSStoreDataParser::countContentItems() {
    QString result;

    QXmlQuery query;
    bool hasFocus = query.setFocus(QUrl(target));
    if (hasFocus) {
        query.setQuery("/ocs/data/count(content)");
        if (query.isValid())
            query.evaluateTo(&result);
    } else
            emit error(QString("Unable to open: %1").arg(target.toString()));

    return result.toInt();
}

void OCSStoreDataParser::parseContentTag(int idx) {
    QString name = getContentStringField(idx, "name");
    QString version = getContentStringField(idx, "version");

    QString description = getContentStringField(idx, "description");
    QString autor = getContentStringField(idx, "personid");
    QString icon = getContentStringField(idx, "smallpreviewpic1");

    int appimageDownloadIdx = getAppimageDownloadIdx(idx);
    QString downloadLink = getContentStringField(idx, QString("downloadlink%1").arg(appimageDownloadIdx));
    QString downloadSizeStr = getContentStringField(idx, QString("downloadsize%1").arg(appimageDownloadIdx));

    QStringList screenShots = getScreenShots(idx);

    Application application(name, version);
    application.setDescription(description);
    application.setAuthors({autor});
    application.setIcon(icon);
    application.setDownloadUrl(downloadLink);
    application.setDownloadSize(downloadSizeStr.toInt());
    application.setScreenshots(screenShots);

    if (!application.isEmpty())
        results << application;
}

QStringList OCSStoreDataParser::getScreenShots(int contentIdx) const {
    QString text;
    QXmlQuery query;
    query.setFocus(target);
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

int OCSStoreDataParser::getAppimageDownloadIdx(int contentIdx) const {
    int appimageDownloadIdx;
    QString text;
    QXmlQuery query;
    query.setFocus(target);
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
    QXmlQuery query;
    query.setFocus(target);
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
