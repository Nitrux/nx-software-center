#include "appimagehubrepository.h"

#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "../entities/app.h"
#include "../entities/release.h"

AppImageHubRepository::AppImageHubRepository(QString url)
{
    m_dataUrl = QUrl(url);
}

QString AppImageHubRepository::id()
{
    return "AppImageHub";
}

void AppImageHubRepository::updateCache()
{
    if (m_isUpdating)
        return;

    m_isUpdating = true;
    auto request = QNetworkRequest(m_dataUrl);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    m_networkReply = m_networkAccessManager.get(request);
    QObject::connect(m_networkReply, &QNetworkReply::finished, [=]()
    {
        handleNetworkReply();
    });
}


/**
 * This is a best effort find download links function it's supposed to work
 * for github releases but not for other sites.
 * It works asynchronously when finished the 'download' at the appImage will
 * be updated.
 **/
void AppImageHubRepository::findDownloadLinks(App *app, QString arch)
{
    Q_ASSERT(app != nullptr);

    if (app->links.contains("install"))
    {
        QString url = app->links.value("install");
        auto request = QNetworkRequest(url);
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

        auto reply = m_networkAccessManager.get(request);
        QObject::connect(reply, &QNetworkReply::finished, [=]()
        {
            if (reply->error() == QNetworkReply::NoError)
            {
                QString rxString = "href=\"([^\"]*%1[^\"]*AppImage)\"";
                rxString = rxString.arg(arch);
                QRegExp rx(rxString);
                QString str = reply->readAll();
                QStringList list;
                int pos = 0;

                while ((pos = rx.indexIn(str, pos)) != -1)
                {
                    QString capUrl = rx.cap(1);
                    if (!capUrl.startsWith("http"))
                    {
                        QUrl url = reply->url();
                        url.setPath(capUrl);
                        capUrl = url.toString();
                    }

                    list << capUrl ;
                    pos += rx.matchedLength();

                    rx.capturedTexts();
                    break;
                }

                if (list.count() > 0) {
                    auto release = app->lastRelease();
                    release->download_link = list.first();
                }
            } else
            {
                qDebug() << reply->errorString() << ": while resolving download link for " << app->id;
            }
        });
    } else
    {
        qDebug() << app->id << " has no install link ";
    }
}

void AppImageHubRepository::handleNetworkReply()
{
    if (m_networkReply != nullptr)
    {
        qDebug() << "Reply" << m_networkReply->header(QNetworkRequest::ContentLengthHeader) << m_networkReply->header(QNetworkRequest::ContentTypeHeader);
        if (m_networkReply->error() == QNetworkReply::NoError)
        {
            QByteArray rawData = m_networkReply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(rawData);
            auto jsonRoot = doc.object();
            if (jsonRoot.contains("items"))
            {
                QList<App *> newItems;
//                qDebug() << "Loading appimage items";
                m_data = jsonRoot.value("items").toArray();
                for (QJsonValue appImageData: m_data)
                {
                    QJsonObject appImageDataObject = appImageData.toObject();

                    App * appImage = new App(appImageDataObject.value("name").toString());
                    Release *release = new Release("latest", appImage->id);
                    release->name = appImageDataObject.value("name").toString();
                    release->description = appImageDataObject.value("description").toString();
                    release->licence = appImageDataObject.value("licence").toString();

                    QStringList authors;
                    for (QJsonValue value: appImageDataObject.value("autors").toArray())
                        authors << value.toString();
                    release->authors = authors;

                    QStringList categories;
                    for (QJsonValue value: appImageDataObject.value("categotries").toArray())
                        categories << value.toString();
                    appImage->categories = categories;

                    QStringList screenshots;
                    for (QJsonValue value: appImageDataObject.value("screenshots").toArray())
                        screenshots << value.toString();
                    release->screenshot_links = screenshots;


                    QMap<QString, QString> links;
                    auto linksArray = appImageDataObject.value("links").toArray();
                    for (QJsonValue linkItem: linksArray)
                    {
                        auto linkObject = linkItem.toObject();
                        auto key = linkObject.value("type").toString().toLower();
                        auto value = linkObject.value("url").toString();

                        links.insert(key, value);
                    }
                    appImage->links = links;

                    appImage->addRelease(release);

                    findDownloadLinks(appImage, QSysInfo::currentCpuArchitecture());

                    apps.insert(appImage->id, appImage);
                }

//                qDebug() << newItems.count() << " appimage items loaded";
            } else
            {
                qWarning() << "No \"items\" array found in reply";
            }
        } else
        {
            qDebug() << "Something went wrong: " << m_networkReply->errorString();
        }
    }

    m_networkReply->deleteLater();
    m_networkReply = nullptr;
    m_isUpdating = false;
}
