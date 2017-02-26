#ifndef CUSTOMNETWORKACCESSMANAGER_H
#define CUSTOMNETWORKACCESSMANAGER_H

#include <QList>
#include <QObject>
#include <QPointer>
#include <QNetworkProxy>
#include <QQmlNetworkAccessManagerFactory>

class SnapdSettings;

class CustomNetworkAccessManagerFactory : public QObject, public QQmlNetworkAccessManagerFactory
{
    Q_OBJECT
public:
    explicit CustomNetworkAccessManagerFactory(SnapdSettings * snapdSettings);
    virtual QNetworkAccessManager *create(QObject *parent);

public slots:
    void updateProxies();

private:
    QNetworkProxy createProxy();
    SnapdSettings * m_settings;
    QList<QPointer<QNetworkAccessManager>> m_instances;
};

#endif // CUSTOMNETWORKACCESSMANAGER_H
