#ifndef DUMMYDOWNLOADMANAGER_H
#define DUMMYDOWNLOADMANAGER_H

#include <QString>

#include "../entities/downloadmanager.h"

class DummyDownloadManager : public DownloadManager
{
public:
    DummyDownloadManager(bool fail = false);
    virtual ~DummyDownloadManager();

    virtual void download(const QString &url, QString destination, ProgressListener * listener) override;
    virtual void brokenDownload(const QString &url, QString destination, ProgressListener * listener);

    void createTmpFile(QString filePath);
    void removeTmpFile(QString filePath);

    bool m_fail;
};

#endif // DUMMYDOWNLOADMANAGER_H
