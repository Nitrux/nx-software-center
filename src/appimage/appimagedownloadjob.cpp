#include "appimagedownloadjob.h"

#include <QNetworkReply>
#include <KIO/TransferJob>

AppImageDownloadJob::AppImageDownloadJob(QString url, QObject *parent): KJob(parent)
{
    m_url = QUrl(url);
}

QUrl AppImageDownloadJob::url() const
{
    return m_url;
}

void AppImageDownloadJob::start()
{
    m_file = new QTemporaryFile;
    m_file->open();
    auto job = KIO::get(m_url);

    connect(job, &KIO::TransferJob::infoMessage, [=] (KJob *job, const QString &plain, const QString &rich)
    {
        Q_UNUSED(job)
        Q_UNUSED(rich)
        if (!plain.isEmpty())
            setStatus(plain);
    });

    connect(job, &KIO::TransferJob::data, [=] (KIO::Job *job, const QByteArray &data)
    {
        m_file->write(data);
        setPercent(job->percent());
    });

    connect(job, &KIO::TransferJob::finished, [=] (KJob *job)
    {
        setError(job->error());
        setErrorText(job->errorText());
        emitResult();
        if (job->error())
        {
            qDebug() << "Algo fue mal" << job->errorString();
        } else
        {
            qDebug() << "All ok";
        }
        m_file->close();
        m_file->deleteLater();
    });

    job->start();
}

QString AppImageDownloadJob::status() const
{
    return m_status;
}

void AppImageDownloadJob::setUrl(QUrl url)
{
    if (m_url == url)
        return;

    m_url = url;
    emit urlChanged(m_url);
}

void AppImageDownloadJob::setStatus(QString status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(m_status);
}
