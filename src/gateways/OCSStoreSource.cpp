//
// Created by alexis on 3/7/18.
//

#include <QUrlQuery>
#include <QtCore/QRunnable>
#include "OCSStoreSource.h"

OCSStoreSource::OCSStoreSource(const QUrl &url, QObject *parent)
        : Source(parent), url(url) {
    Q_ASSERT(!url.isEmpty());

    connect(&parser, &OCSStoreDataParser::completed, this, &OCSStoreSource::handleParserCompleted, Qt::DirectConnection);
    connect(&parser, &OCSStoreDataParser::error, this, &OCSStoreSource::handleParserError, Qt::DirectConnection);
}

void OCSStoreSource::fetchAllApplications() {
    page = 0;
    QUrlQuery query = getQuery();
    startParser(query);
}

void OCSStoreSource::startParser(const QUrlQuery &query) {
    QUrl newUrl = url;
    newUrl.setQuery(query);
    parser.setTarget(newUrl);
    parser.extractApplications();
}

QUrlQuery OCSStoreSource::getQuery() const {
    QUrlQuery query;
    query.addQueryItem("package_types", "1");
    query.addQueryItem("page", QString::number(page));
    return query;
}

void OCSStoreSource::handleParserCompleted() {
    applications.append(parser.getResults());

    int itemsPerPage = parser.getItemsPerPage();
    int totalItems = parser.getTotalItems();

    if (page * itemsPerPage < totalItems) {
        page++;
        QUrlQuery query = getQuery();
        startParser(query);
    } else
            emit fetchedAllApplications(applications);

}

void OCSStoreSource::handleParserError(const QString &msg) {
    emit fetchError(msg);
}
