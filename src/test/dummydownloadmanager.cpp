#include "dummydownloadmanager.h"

#include <QFile>
#include "../entities/progresslistener.h"

DummyDownloadManager::DummyDownloadManager(bool fail)
{
    m_fail = fail;
}

DummyDownloadManager::~DummyDownloadManager()
{
}

void DummyDownloadManager::download(const QString &url, QString destination, ProgressListener *listener)
{
    if (m_fail)
        return brokenDownload(url, destination, listener);

    Q_ASSERT(listener != nullptr);

    createTmpFile(destination);

    listener->progress(0, 100, "Starting download");
    listener->progress(50, 100, "Almost there");
    listener->progress(100, 100, "Complete");
}

void DummyDownloadManager::brokenDownload(const QString &url, QString destination, ProgressListener *listener)
{
    Q_ASSERT(listener != nullptr);

    listener->progress(0, 100, "Starting download");
    listener->progress(50, 100, "Almost there");

    listener->error("Something went wrong.");
}

void DummyDownloadManager::createTmpFile(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.write("test");
    file.close();
}

void DummyDownloadManager::removeTmpFile(QString filePath)
{
    QFile file(filePath);
    file.remove();
}
