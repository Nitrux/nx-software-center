#ifndef KF5DOWNLOADMANAGER_H
#define KF5DOWNLOADMANAGER_H

#include "../entities/downloadmanager.h"

class KF5DownloadManager : public DownloadManager
{
public:
    KF5DownloadManager();

    virtual QString download(const QString &url, ProgressListener * listener);
};

#endif // KF5DOWNLOADMANAGER_H
