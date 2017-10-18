#include "appimage.h"

AppImage::AppImage(QObject *parent): QObject(parent)
{

}

QString AppImage::id() const
{
    return m_id;
}

QVariantHash AppImage::data() const
{
    return m_data;
}

QString AppImage::getName()
{
    if (m_data.contains("name"))
        return m_data.value("name").toString();

    return m_id;
}

QString AppImage::getDescription()
{
    return m_data.value("description", "").toString();
}

QString AppImage::getGithubLink()
{
    return m_data.value("github").toString();
}

QString AppImage::getInstallLink()
{
    return m_data.value("install").toString();
}

QString AppImage::getDownloadLink(QString architecture)
{
    return m_data.value("download_" + architecture, "").toString();
}

QVariantHash AppImage::links() const
{
    return m_links;
}

QVariantList AppImage::authors() const
{
    return m_authors;
}

void AppImage::setId(QString id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

void AppImage::setData(QVariantHash data)
{
    if (m_data == data)
        return;

    m_data = data;
    emit dataChanged(m_data);
}

void AppImage::setLinks(QVariantHash links)
{
    if (m_links == links)
        return;

    m_links = links;
    emit linksChanged(m_links);
}

void AppImage::setAuthors(QVariantList authors)
{
    if (m_authors == authors)
        return;

    m_authors = authors;
    emit authorsChanged(m_authors);
}
