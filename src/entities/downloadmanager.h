#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QString>

class ProgressListener;

class DownloadManager
{
public:
    virtual ~DownloadManager() {}
    virtual void download(const QString &url, const QString destination, ProgressListener * listener) = 0;
};

#endif // DOWNLOADMANAGER_H
