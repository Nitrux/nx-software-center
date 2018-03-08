//
// Created by alexis on 3/7/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_OCSSOURCE_H
#define NOMAD_SOFTWARE_CENTER_OCSSOURCE_H

#include <QUrl>
#include <QUrlQuery>
#include <QList>
#include <QThread>

#include <entities/Source.h>
#include <entities/Application.h>

#include "DownloadManager.h"
#include "OCSStoreDataParser.h"

class OCSStoreSource : public Source {
Q_OBJECT
    QUrl url;

    int page;
    OCSStoreDataParser parser;
    QList<Application> applications;
public:
    OCSStoreSource(const QUrl &url, QObject *parent);

public slots:

    void fetchAllApplications() override;

private slots:

    void handleParserCompleted();

    void handleParserError(const QString &msg);

    QUrlQuery getQuery() const;

    void startParser(const QUrlQuery &query);
};


#endif //NOMAD_SOFTWARE_CENTER_OCSSOURCE_H
