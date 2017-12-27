#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QString>
#include <QByteArray>
#include <QObject>

class DownloadJob : public QObject {
Q_OBJECT
public:
    DownloadJob(QObject *parent) : QObject(parent) {}

    virtual void execute() = 0;

signals:

    void progress(const int value, const int total, const QString &message);

    void finished();

    void error(const QString &error);

};

class DownloadToMemoryJob : public DownloadJob {
Q_OBJECT
protected:
    QByteArray data;
public:
    DownloadToMemoryJob(QObject *parent) : DownloadJob(parent) {};

    const QByteArray &getData() const { return data; };
};

class DownloadToFileJob : public DownloadJob {
Q_OBJECT
protected:
    QString path;
public:
    DownloadToFileJob(const QString &path, QObject *parent) : DownloadJob(parent), path(path) {};

    const QString &getPath() const { return path; }
};

class DownloadManager : public QObject {
Q_OBJECT
public:
    DownloadManager(QObject *parent) : QObject(parent) {}

    virtual ~DownloadManager() {}

    virtual DownloadToFileJob *downloadToFile(const QString &url, const QString &path) = 0;

    virtual DownloadToMemoryJob *downloadToMemory(const QString &url) = 0;
};

#endif // DOWNLOADMANAGER_H
