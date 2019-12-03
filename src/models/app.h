#ifndef APP_H
#define APP_H

#include <QObject>

#ifdef STATIC_MAUIKIT
#include "fmh.h"
#else
#include <MauiKit/fmh.h>
#endif

class Application;
class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Application * data WRITE setData READ getData NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap info MEMBER m_info NOTIFY infoChanged)
    Q_PROPERTY(QVariantList downloads MEMBER m_downloads NOTIFY downloadsChanged)
    Q_PROPERTY(QVariantList images MEMBER m_images NOTIFY imagesChanged)
    Q_PROPERTY(QVariantList urls MEMBER m_urls NOTIFY urlsChanged)

    Q_PROPERTY(bool isInstalled MEMBER m_isInstalled NOTIFY isInstalledChanged)
    Q_PROPERTY(bool isUpdatable MEMBER m_isUpdatable NOTIFY isUpdatableChanged)

public:
    explicit App(QObject *parent = nullptr);

    Application * getData() const;

private:
    Application * m_data;
    QVariantMap m_info;
    QVariantList m_downloads;
    QVariantList m_images;

    void setModels();

    QVariantList m_urls;

    bool m_isInstalled = false;
    bool m_isUpdatable = false;

signals:

    void infoChanged(QVariantMap info);
    void downloadsChanged(QVariantList downloads);
    void imagesChanged(QVariantList images);

    void urlsChanged(QVariantList urls);
    void isInstalledChanged(bool isInstalled);
    void isUpdatableChanged(bool isUpdatable);
    void dataChanged(Application * data);

public slots:
    void updateApp();
    void removeApp();
    void installApp(const int &packageIndex);
    void launchApp();
    void buyApp();
    void setData(Application * data);
};

#endif // APP_H
