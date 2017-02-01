#ifndef SNAPDSETTINGS_H
#define SNAPDSETTINGS_H

#include <QObject>
#include <KAuth/KAuthExecuteJob>

class SnapdSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool useProxy READ useProxy WRITE setUseProxy NOTIFY useProxyChanged)
    Q_PROPERTY(QString httpProxy READ httpProxy WRITE setHttpProxy NOTIFY httpProxyChanged)
    Q_PROPERTY(int httpProxyPort READ httpProxyPort WRITE setHttpProxyPort NOTIFY httpProxyPortChanged)
    Q_PROPERTY(QString httpsProxy READ httpsProxy WRITE setHttpsProxy NOTIFY httpsProxyChanged)
    Q_PROPERTY(int httpsProxyPort READ httpsProxyPort WRITE setHttpsProxyPort NOTIFY httpsProxyPortChanged)
    Q_PROPERTY(QString noProxy READ noProxy WRITE setNoProxy NOTIFY noProxyChanged)

    Q_PROPERTY(Store store READ store WRITE setStore NOTIFY storeChanged)
    Q_PROPERTY(QString customStoreUrl READ customStoreUrl WRITE setCustomStoreUrl NOTIFY customStoreUrlChanged)

public:
    SnapdSettings();
    enum Store { Ubuntu, Custom};
    Q_ENUM(Store)


    bool useProxy() const;
    QString httpsProxy() const;
    QString httpProxy() const;
    QString noProxy() const;
    Store store() const;
    QString customStoreUrl() const;

    int32_t httpProxyPort() const;

    int32_t httpsProxyPort() const;

public slots:
    void setUseProxy(bool useProxy);
    void setHttpsProxy(QString httpsProxy);
    void setHttpProxy(QString httpProxy);
    void setNoProxy(QString noProxy);
    void setStore(Store store);
    void setCustomStoreUrl(QString customStoreUrl);

    Q_INVOKABLE void load();
    Q_INVOKABLE KAuth::ExecuteJob *apply();


    void setHttpProxyPort(int httpProxyPort);
    void setHttpsProxyPort(int httpsProxyPort);

signals:
    void useProxyChanged(bool useProxy);
    void httpsProxyChanged(QString httpsProxy);
    void httpProxyChanged(QString httpProxy);
    void noProxyChanged(QString noProxy);
    void storeChanged(Store store);
    void customStoreUrlChanged(QString customStoreUrl);

    void httpProxyPortChanged(int httpProxyPort);

    void httpsProxyPortChanged(int httpsProxyPort);

private:
    bool m_useProxy;
    QString m_httpsProxy;
    QString m_httpProxy;
    QString m_noProxy;

    Store m_store;
    QString m_customStoreUrl;
    int m_httpProxyPort;
    int m_httpsProxyPort;
};

#endif // SNAPDSETTINGS_H
