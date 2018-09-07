//
// Created by alexis on 7/10/18.
//

#include "DummyApplicationRepository.h"
#include "DummyApplicationRepositoryGet.hpp"
#include "DummyApplicationRepositorySearch.hpp"
#include <gateways/FileDownload.h>

ApplicationsRepositorySearch *
DummyApplicationRepository::buildSearchRequest(const QString &query, const QString &category) const {
    return new DummyApplicationRepositorySearch();
}

ApplicationRepositoryGet *DummyApplicationRepository::buildGetApplicationRequest(const QString &id) const {
    return new DummyApplicationRepositoryGet();
}

FileDownload * DummyApplicationRepository::buildFileDownloadRequest(QString url, QString path) {
    auto fileDownload = new FileDownload(url, path);
    fileDownload->setNetworkAccessManager(new QNetworkAccessManager());
    fileDownload->setProgressNotificationsEnabled(true);
    return fileDownload;
}
