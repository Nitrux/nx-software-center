//

#include <QNetworkRequest>

#include "ApplicationRepositoryRestClient.h"
#include "ApplicationsSearchRequest.h"
#include "ApplicationGetRequest.h"
#include "FileDownload.h"

ApplicationRepositoryRestClient::ApplicationRepositoryRestClient(QString url, QObject *parent)
        : QObject(parent), ApplicationRepository(), api(url), networkAccessManager(new QNetworkAccessManager(this)) {

}

ApplicationsRepositorySearch *
ApplicationRepositoryRestClient::buildSearchRequest(const QString &query, const QString &category) const {
    auto request = new ApplicationsSearchRequest();
    request->setNetworkAccessManager(networkAccessManager);
    request->setApi(api);
    request->setQuery(query);
    request->setCategory(category);

    return request;
}

ApplicationGetRequest *ApplicationRepositoryRestClient::buildGetApplicationRequest(const QString &id) const {
    auto request = new ApplicationGetRequest();
    request->setNetworkAccessManager(networkAccessManager);
    request->setApi(api);
    request->setId(id);

    return request;
}

void ApplicationRepositoryRestClient::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager) {
    ApplicationRepositoryRestClient::networkAccessManager = networkAccessManager;
}

FileDownload * ApplicationRepositoryRestClient::buildFileDownloadRequest(QString url, QString path) {
    auto fileDownload = new FileDownload(url, path);
    fileDownload->setNetworkAccessManager(networkAccessManager);
    fileDownload->setProgressNotificationsEnabled(true);
    return fileDownload;
}
