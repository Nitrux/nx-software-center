#ifndef APP_H
#define APP_H

#include <QObject>
#include <MauiKit/Core/fmh.h>

class PlingStoreApplicationData;
class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PlingStoreApplicationData * data WRITE setData READ getData NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap info MEMBER m_info NOTIFY infoChanged)
    Q_PROPERTY(QVariantList downloads MEMBER m_downloads NOTIFY downloadsChanged)
    Q_PROPERTY(QVariantList images MEMBER m_images NOTIFY imagesChanged)
    Q_PROPERTY(QVariantList urls MEMBER m_urls NOTIFY urlsChanged)
    Q_PROPERTY(QString id MEMBER m_id NOTIFY idChanged CONSTANT FINAL)
    Q_PROPERTY(QString localPath MEMBER m_localPath NOTIFY localPathChanged CONSTANT FINAL)
    Q_PROPERTY(bool isInstalled MEMBER m_isInstalled NOTIFY isInstalledChanged)
    Q_PROPERTY(bool isUpdatable MEMBER m_isUpdatable NOTIFY isUpdatableChanged)


public:
    explicit App(QObject *parent = nullptr);
    App(const App &other, QObject *parent = nullptr);

    PlingStoreApplicationData * getData() const;

    PlingStoreApplicationData * m_data;

    QVariantMap m_info;
    QVariantList m_downloads;
    QVariantList m_images;
    QVariantList m_urls;

    QString m_id;
    QString m_localPath;

    bool m_isInstalled = false;
    bool m_isUpdatable = false;

    void setModels();

signals:
    void infoChanged(QVariantMap info);
    void downloadsChanged(QVariantList downloads);
    void imagesChanged(QVariantList images);

    void urlsChanged(QVariantList urls);
    void isInstalledChanged(bool isInstalled);
    void isUpdatableChanged(bool isUpdatable);
    void dataChanged(PlingStoreApplicationData * data);

    void idChanged(QString id);

    void localPathChanged(QString localPath);

public slots:
    void setData(PlingStoreApplicationData * data);
};

#endif // APP_H
