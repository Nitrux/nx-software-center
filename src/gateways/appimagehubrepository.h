#ifndef APPIMAGEHUBREPOSITORY_H
#define APPIMAGEHUBREPOSITORY_H

#include <QUrl>
#include <QJsonArray>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "../entities/repository.h"

class App;
class AppImageHubRepository : public Repository
{
public:
    AppImageHubRepository(QString url);

    virtual QString id() override;
    virtual void updateCache() override;

protected:
    void handleNetworkReply();
    void findDownloadLinks(App *app, QString arch);

    QUrl m_dataUrl;
    QJsonArray m_data;
    QNetworkAccessManager m_networkAccessManager;

    bool m_isUpdating = false;
    QNetworkReply * m_networkReply = nullptr;
};

#endif // APPIMAGEHUBREPOSITORY_H
