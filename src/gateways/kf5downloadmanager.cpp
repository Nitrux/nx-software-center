#include "kf5downloadmanager.h"

#include <QUrl>
#include <QTemporaryFile>
#include <KIO/TransferJob>

#include "../entities/progresslistener.h"

KF5DownloadManager::KF5DownloadManager()
{

}

QString KF5DownloadManager::download(const QString &url, ProgressListener *listener)
{
    QTemporaryFile *m_file = new QTemporaryFile;
    m_file->setAutoRemove(false);
    m_file->open();
    auto job = KIO::get(url);

    QObject::connect(job, &KIO::TransferJob::infoMessage, [=] (KJob *job, const QString &plain, const QString &rich)
    {
        Q_UNUSED(rich)
        listener->progress(job->processedAmount(KJob::Bytes), job->totalAmount(KJob::Bytes), plain);
    });

    QObject::connect(job, &KIO::TransferJob::data, [=] (KIO::Job *job, const QByteArray &data)
    {
        m_file->write(data);
        QString message = "Downloading AppImage file...";
        listener->progress(job->processedAmount(KJob::Bytes), job->totalAmount(KJob::Bytes), message);
    });

    QObject::connect(job, &KIO::TransferJob::finished, [=] (KJob *job)
    {
        if (job->error())
            listener->error(job->errorText());
        else
            listener->finished();

        m_file->close();
        m_file->deleteLater();

        job->deleteLater();
    });

    QString fileName = m_file->fileName();
    job->exec();

    return fileName;
}
