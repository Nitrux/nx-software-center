#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QString>

class ProgressListener;

class DownloadManager
{
public:
    virtual ~DownloadManager() {}
    virtual QString download(const QString &url, ProgressListener * listener) = 0;
};

#endif // DOWNLOADMANAGER_H
