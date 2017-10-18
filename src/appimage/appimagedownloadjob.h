#ifndef APPIMAGEDOWNLOADJOB_H
#define APPIMAGEDOWNLOADJOB_H

#include <QUrl>
#include <QTemporaryFile>

#include <KJob>

class AppImageDownloadJob : public KJob
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)

public:
    AppImageDownloadJob(QString url, QObject *parent = nullptr);
    QUrl url() const;

    virtual void start();
    QString status() const;

public slots:
    void setUrl(QUrl url);

protected slots:
    void setStatus(QString status);

signals:
    void urlChanged(QUrl url);
    void statusChanged(QString status);

private:
    QUrl m_url;
    QString m_status;
    QTemporaryFile *m_file = nullptr;
};

#endif // APPIMAGEDOWNLOADJOB_H
