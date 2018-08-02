//
// Created by alexis on 7/10/18.
//

#ifndef NX_SOFTWARE_CENTER_DUMMYAPPLICATIONREGISTRY_H
#define NX_SOFTWARE_CENTER_DUMMYAPPLICATIONREGISTRY_H

#include <gateways/ApplicationRepository.h>

class DummyApplicationRepository : public ApplicationRepository {
public:
    ApplicationsRepositorySearch *buildSearchRequest(const QString &query, const QString &category) const override;

    ApplicationRepositoryGet *buildGetApplicationRequest(const QString &id) const override;

    FileDownload * buildFileDownloadRequest(QString url, QString path) override;

};


#endif //NX_SOFTWARE_CENTER_DUMMYAPPLICATIONREGISTRY_H
