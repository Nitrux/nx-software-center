//
// Created by alexis on 7/10/18.
//

#ifndef NX_SOFTWARE_CENTER_APPLICATIONREPOSITORY_H
#define NX_SOFTWARE_CENTER_APPLICATIONREPOSITORY_H


#include <QtCore/QString>
#include "ApplicationGetRequest.h"
#include "ApplicationsRepositorySearch.h"
#include "FileDownload.h"

class ApplicationRepository {
public:
    virtual ~ApplicationRepository();

    virtual ApplicationsRepositorySearch *buildSearchRequest(const QString &query, const QString &category) const = 0;

    virtual ApplicationRepositoryGet *buildGetApplicationRequest(const QString &id) const = 0;

    virtual FileDownload * buildFileDownloadRequest(QString url, QString string) = 0;

};


#endif //NX_SOFTWARE_CENTER_APPLICATIONREPOSITORY_H
