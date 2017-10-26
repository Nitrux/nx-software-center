#ifndef DUMMYDOWNLOADMANAGER_H
#define DUMMYDOWNLOADMANAGER_H

#include <QString>

#include "../entities/downloadmanager.h"

class DummyDownloadManager : public DownloadManager
{
public:
    DummyDownloadManager(bool fail = false);
    virtual ~DummyDownloadManager();

    virtual QString download(const QString &url, ProgressListener * listener);
    virtual QString brokenDownload(const QString &url, ProgressListener * listener);

    void createTmpFile(QString filePath);
    void removeTmpFile();

    bool m_fail;
};

#endif // DUMMYDOWNLOADMANAGER_H
