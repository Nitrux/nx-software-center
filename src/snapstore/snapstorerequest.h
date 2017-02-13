#ifndef SNAPSTOREREQUEST_H
#define SNAPSTOREREQUEST_H

#include <QObject>

class SnapStoreRequest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString response READ response NOTIFY responseReady)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QString storeUrl READ storeUrl)

public:
    explicit SnapStoreRequest(const QString &storeUrl, QObject *parent = 0);

    Q_INVOKABLE virtual void start()  = 0;
    Q_INVOKABLE virtual void cancel()  = 0;
    virtual QString response() const;

    QString errorString() const;

    QString storeUrl() const;

signals:
    void finished();
    void responseReady(QString response);
    void errorStringChanged(QString errorString);

protected:
    QString m_storeUrl;
    QString m_response;
    QString m_errorString;
};

#endif // SNAPSTOREREQUEST_H
