#include "appimagehubrepository.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QBuffer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSysInfo>

#include "appimage.h"

AppImageHubRepository::AppImageHubRepository(QString url, QObject *parent) : AppImageRepository(parent)
{
    m_dataUrl = QUrl(url);
    update();
}

int AppImageHubRepository::count()
{
    return m_data.count();
}

QList<AppImage *> AppImageHubRepository::list(int offset, int limit)
{
    return m_items.mid(offset, limit);
}

QVariant AppImageHubRepository::listAsVariant(int offset, int limit)
{
    QList<QObject*>  data;
    for (AppImage * item: m_items.mid(offset, limit))
        data.append(item);
    return QVariant::fromValue(data);
}

QList<AppImage *> AppImageHubRepository::search(QString query, int offset, int limit)
{
    AppImageList selection;
    for (AppImage *appImage: m_items)
        if (appImage->id().contains(query)
                || appImage->description().contains(query)
                || appImage->categories().contains(query) )
            selection.append(appImage);
    return m_items.mid(offset, limit);;
}

QVariant AppImageHubRepository::searchAsVariant(QString query, int offset, int limit)
{

    QList<QObject *> selection;
    for (AppImage *appImage: m_items)
        if (appImage->id().contains(query)
                || appImage->description().contains(query)
                || appImage->categories().contains(query) )
            selection.append(appImage);
    return QVariant::fromValue(m_items.mid(offset, limit));
}

bool AppImageHubRepository::isUpdating() const
{
    return m_isUpdating;
}

/**
 * This is a best effort find download links function it's supposed to work
 * for github releases but not for other sites.
 * It works asynchronously when finished the 'download' at the appImage will
 * be updated.
 **/
void AppImageHubRepository::findDownloadLinks(AppImage * appImage, QString arch)
{
    Q_ASSERT(appImage != nullptr);

    if (appImage->links().contains("install"))
    {
        QString url = appImage->links().value("install").toString();
        auto request = QNetworkRequest(url);
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

        auto reply = m_networkAccessManager.get(request);
        connect(reply, &QNetworkReply::finished, [=]()
        {
            if (reply->error() == QNetworkReply::NoError)
            {
                /* QXmlQuery will be the ideal solution but is unmantained,
                 * letting the code bellow only for future reference */
                //                QXmlQuery query;
                //                query.bindVariable("source", reply);
                //                QString queryString = "doc($source)//a/text()";
                //                query.setQuery(queryString);
                //                query.setQuery("a/@href[contains(., \"AppImage\")]");
                //                QString result;
                //                if (!query.evaluateTo(&result))
                //                    qWarning() << "Something went wrong while evaliuanting the xpath";
                //                qDebug() << result;

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
                    auto links = appImage->links();
                    links["download"] = list.first();
                    appImage->setLinks(links);
                }
            } else
            {
                qDebug() << reply->errorString() << ": while resolving download link for " << appImage->id();
            }
        });
    } else
    {
        qDebug() << appImage->id() << " has no install link ";
    }
}

void AppImageHubRepository::update()
{
    if (m_isUpdating)
        return;

    setIsUpdating(true);
    auto request = QNetworkRequest(m_dataUrl);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    m_networkReply = m_networkAccessManager.get(request);
    connect(m_networkReply, &QNetworkReply::finished, this, &AppImageHubRepository::handleNetworkReply);
}

void AppImageHubRepository::setIsUpdating(bool isUpdating)
{
    if (m_isUpdating == isUpdating)
        return;

    m_isUpdating = isUpdating;
    emit isUpdatingChanged(m_isUpdating);
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
                QList<AppImage *> newItems;
//                qDebug() << "Loading appimage items";
                m_data = jsonRoot.value("items").toArray();
                for (QJsonValue appImageData: m_data)
                {
                    QJsonObject appImageDataObject = appImageData.toObject();

                    AppImage * appImage = new AppImage(this);
                    appImage->setId(appImageDataObject.value("name").toString());
                    appImage->setDescription(appImageDataObject.value("description").toString());
                    appImage->setLicence(appImageDataObject.value("licence").toString());
                    appImage->setAuthors(appImageDataObject.value("autors").toVariant().toList());

                    QStringList categories;
                    for (QJsonValue value: appImageDataObject.value("categotries").toArray())
                        categories << value.toString();
                    appImage->setCategories(categories);

                    QStringList screenshots;
                    for (QJsonValue value: appImageDataObject.value("screenshots").toArray())
                        screenshots << value.toString();
                    appImage->setScreenshots(screenshots);


                    QVariantHash links;
                    auto linksArray = appImageDataObject.value("links").toArray();
                    for (QJsonValue linkItem: linksArray)
                    {
                        auto linkObject = linkItem.toObject();
                        auto key = linkObject.value("type").toString().toLower();
                        auto value = linkObject.value("url").toString();

                        links.insert(key, value);
                    }
                    appImage->setLinks(links);

                    newItems.append(appImage);
                    findDownloadLinks(appImage, QSysInfo::currentCpuArchitecture());
                }

//                qDebug() << newItems.count() << " appimage items loaded";

                setItems(newItems);
            } else
            {
                qWarning() << "No \"items\" array found in reply";
            }
        } else
        {
            qDebug() << "Something went wrong: " << m_networkReply->errorString();
            emit error("Unable to fetch data from appimagehub.io");
        }
    }

    m_networkReply->deleteLater();
    m_networkReply = nullptr;
    setIsUpdating(false);
}
