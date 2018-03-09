//
// Created by alexis on 3/6/18.
//
#include <QDebug>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QXmlResultItems>

#include "OCSStoreDataParser.h"


OCSStoreDataParser::OCSStoreDataParser(QObject *parent) : QObject(parent), itemsPerPage(0), totalItems(0),
                                                          failed(false), queryInstance(nullptr) {
}

OCSStoreDataParser::~OCSStoreDataParser() { delete queryInstance; }

void OCSStoreDataParser::extractApplications() {
    results.clear();

    int contentItems = countContentItems();

    if (!failed) {
        totalItems = evaluateQuery("/ocs/meta/totalitems/text()").toInt();
        itemsPerPage = evaluateQuery("/ocs/meta/itemsperpage/text()").toInt();

        for (int i = 1; i <= contentItems; i++)
            parseContentTag(i);

        emit completed();
    }

    delete queryInstance;
    queryInstance = nullptr;
}

int OCSStoreDataParser::countContentItems() {
    QString result;

    QXmlQuery *query = getQuery();

    query->setQuery("/ocs/data/count(content)");
    if (query->isValid())
        query->evaluateTo(&result);
    else {
        emit error(QString("Unable to open: %1").arg(target.toString()));
        failed = true;
    }

    return result.toInt();
}

QXmlQuery *OCSStoreDataParser::getQuery() {
    if (!queryInstance) {
        queryInstance = new QXmlQuery();
        queryInstance->setFocus(target);
    }
    return queryInstance;
}

void OCSStoreDataParser::parseContentTag(int idx) {
    const QString name = parseContentName(idx);
    const QString codeName = getCodeName(name);
    const QString version = getContentStringField(idx, "version");

    int appimageDownloadIdx = getAppimageDownloadIdx(idx);
    const QString downloadLink = getContentStringField(idx, QString("downloadlink%1").arg(appimageDownloadIdx));
    const QString downloadSizeStr = getContentStringField(idx, QString("downloadsize%1").arg(appimageDownloadIdx));

    if (codeName.isEmpty() || downloadLink.isEmpty())
        return;

    const QString description = getContentStringField(idx, "description");
    const QString autor = getContentStringField(idx, "personid");
    const QString icon = getContentStringField(idx, "smallpreviewpic1");

    const QStringList screenShots = getScreenShots(idx);

    Application application(codeName, version);
    application.setName(name);
    application.setDescription(description);
    application.setAuthors({autor});
    application.setIcon(icon);
    application.setDownloadUrl(downloadLink);
    application.setDownloadSize(downloadSizeStr.toInt());
    application.setScreenshots(screenShots);

    results << application;
}

QString OCSStoreDataParser::getCodeName(const QString &name) const {
    QString codeName = name;
    codeName.replace(" ","-").toLower();
    return codeName;
}

QString OCSStoreDataParser::parseContentName(int idx) {
    QString name = getContentStringField(idx, "name");
    name = name.replace("-", " ").trimmed();
    return name;
}

QStringList OCSStoreDataParser::getScreenShots(int contentIdx) {
    QStringList screenShots;
    QString queryStr = "/ocs/data/content[%1]/*[matches(name(), '^previewpic')]/string()";
    auto query = getQuery();
    query->setQuery(queryStr.arg(contentIdx));

    if (query->isValid())
        query->evaluateTo(&screenShots);
    else
        qWarning() << "Invalid name query.";

    for (QString &screenshot: screenShots)
        screenshot = screenshot.trimmed();

    return screenShots;
}

int OCSStoreDataParser::getAppimageDownloadIdx(int contentIdx) {
    int appimageDownloadIdx;
    QString text;
    QString queryStr = "/ocs/data/content[%1]/*[matches(text(), '.*\\.AppImage') and matches(name(), 'downloadlink.*')]/name()";
    auto query = getQuery();
    query->setQuery(queryStr.arg(contentIdx));

    if (query->isValid())
        query->evaluateTo(&text);
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
    auto query = getQuery();
    query->setQuery(queryString);

    if (query->isValid())
        query->evaluateTo(&text);
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

    delete queryInstance;
    queryInstance = nullptr;
}

bool OCSStoreDataParser::isFailed() const {
    return failed;
}
