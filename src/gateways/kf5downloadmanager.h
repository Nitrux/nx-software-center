#ifndef KF5DOWNLOADMANAGER_H
#define KF5DOWNLOADMANAGER_H

#include "../entities/downloadmanager.h"

class KF5DownloadManager : public DownloadManager
{
public:
    KF5DownloadManager();

    virtual void download(const QString &url, const QString destination, ProgressListener * listener) override;
};

#endif // KF5DOWNLOADMANAGER_H
