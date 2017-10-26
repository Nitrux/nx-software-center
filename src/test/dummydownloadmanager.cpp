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

QString DummyDownloadManager::download(const QString &url, ProgressListener *listener)
{
    if (m_fail)
        return brokenDownload(url, listener);

    Q_ASSERT(listener != nullptr);

    QString filePath = "/tmp/fake_download_file.AppImage";
    createTmpFile(filePath);

    listener->progress(0, 100, "Starting download");
    listener->progress(50, 100, "Almost there");
    listener->progress(100, 100, "Complete");

    return filePath;
}

QString DummyDownloadManager::brokenDownload(const QString &, ProgressListener *listener)
{
    Q_ASSERT(listener != nullptr);

    QString filePath = "/tmp/fake_download_file.AppImage";

    listener->progress(0, 100, "Starting download");
    listener->progress(50, 100, "Almost there");

    listener->error("Something went wrong.");

    return "";
}

void DummyDownloadManager::createTmpFile(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.write("test");
    file.close();
}

void DummyDownloadManager::removeTmpFile()
{
    QString filePath = "/tmp/fake_download_file.AppImage";
    QFile file(filePath);
    file.remove();
}
