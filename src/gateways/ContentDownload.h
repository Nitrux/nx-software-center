//
// Created by alexis on 17/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_CONTENTDOWNLOAD_H
#define NOMAD_SOFTWARE_CENTER_CONTENTDOWNLOAD_H

#include "Download.h"

#include <QByteArray>

class ContentDownload : public Download {
    Q_OBJECT
    QByteArray content;
public:
    ContentDownload(const QString &url, QObject *parent = nullptr);

    const QByteArray &getContent() const;

protected slots:
    void handleFinished() override;
};


#endif //NOMAD_SOFTWARE_CENTER_CONTENTDOWNLOAD_H
