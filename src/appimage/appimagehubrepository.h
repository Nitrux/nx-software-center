#ifndef APPIMAGEHUBREPOSITORY_H
#define APPIMAGEHUBREPOSITORY_H

#include <QUrl>
#include <QObject>
#include <QJsonArray>
#include <QNetworkAccessManager>

#include "appimagerepository.h"

class AppImageHubRepositoryOld : public AppImageRepository
{
    Q_OBJECT
    Q_PROPERTY(bool isUpdating READ isUpdating WRITE setIsUpdating NOTIFY isUpdatingChanged)
public:
    explicit AppImageHubRepositoryOld(QString url, QObject *parent = nullptr);

    Q_INVOKABLE virtual int count();
    Q_INVOKABLE virtual QList<AppImage *> list(int offset = 0, int limit = -1);
    Q_INVOKABLE virtual QVariant listAsVariant(int offset = 0, int limit = -1);

    Q_INVOKABLE virtual QList<AppImage *> search(QString query, int offset = 0, int limit = -1);
    Q_INVOKABLE virtual QVariant searchAsVariant(QString query, int offset = 0, int limit = -1);

    bool isUpdating() const;
    void findDownloadLinks(AppImage *appImage, QString arch);

signals:
    void dataChanged();
    void error(QString msg);

    void isUpdatingChanged(bool isUpdating);

public slots:
    void update();

protected slots:
    void setIsUpdating(bool isUpdating);
    void handleNetworkReply();

private:
    QUrl m_dataUrl;
    QJsonArray m_data;
    QNetworkAccessManager m_networkAccessManager;

    bool m_isUpdating = false;
    QNetworkReply * m_networkReply = nullptr;
};

#endif // APPIMAGEHUBREPOSITORY_H
