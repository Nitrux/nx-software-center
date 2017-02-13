#ifndef SNAPSTOREREQUEST_H
#define SNAPSTOREREQUEST_H

#include <QObject>
#include <QSslError>
#include <QNetworkReply>

class SnapStore;

class SnapStoreRequest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString errorString READ errorString)
    Q_PROPERTY(bool isFinished READ isFinished)
    Q_PROPERTY(int error READ error)

public:
    explicit SnapStoreRequest(SnapStore *store);

    Q_INVOKABLE virtual void runAsync() = 0;
    Q_INVOKABLE virtual void cancel() = 0;

    QString errorString() const;
    bool isFinished() const;
    int error() const;

signals:
    void complete();

public slots:
    void onProgress(qint64 progress, qint64 total);
    void onSslErrors(QList<QSslError> sslErrors);
    void onNetworkErrorResponse(QNetworkReply::NetworkError error);

protected:
    int m_error;
    bool m_isFinished;
    QString m_errorString;
    SnapStore * m_snapStore;
};

#endif // SNAPSTOREREQUEST_H
