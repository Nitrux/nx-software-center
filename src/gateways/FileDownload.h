//
// Created by alexis on 17/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_DOWNLOAD_H
#define NOMAD_SOFTWARE_CENTER_DOWNLOAD_H


#include <QTimer>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QFile>

class FileDownload : public QObject {
Q_OBJECT
    QString source_url;
    bool running;
    bool progressNotificationsEnabled;
    QFile file;

public:
    explicit FileDownload(QString url, QString path, QObject *parent = nullptr);

    virtual ~FileDownload();

    const QString &getSource_url() const;

    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);

    void setProgressNotificationsEnabled(bool progressNotificationsEnabled);

    virtual void start();

    virtual void stop();

    bool isRunning();

    const QString getTargetPath() const;

signals:

    void completed();

    void stopped(const QString &message);

    void progress(qint64 progress, qint64 total, const QString &message);

protected slots:

    virtual void handleDownloadProgress(qint64 bytesRead, qint64 totalBytes);

    virtual void handleTimerTick();

    virtual void handleFinished();

    void handleReadyRead();

    void handleCompleted();

    void handleStopped();

protected:
    bool isStopRequested;
    QNetworkAccessManager *networkAccessManager;

    QSharedPointer<QNetworkReply> reply;
    QTimer timer;
    float speed;
    qint64 totalBytes;
    qint64 bytesRead;

    qint64 bytesReadLastTick;

    static QString formatMemoryValue(float num);

    void createNetworkAccessManagerIfNotExist();

    void reportProgress();

    void updateDownloadSpeed();

    bool wasCompletedProperly() const;

    QNetworkRequest createRequest() const;

    void downloadAvailableBytes();
};


#endif //NOMAD_SOFTWARE_CENTER_DOWNLOAD_H
