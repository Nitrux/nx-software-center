#include "appimage.h"

AppImage::AppImage(QObject *parent): QObject(parent)
{

}

QString AppImage::id() const
{
    return m_id;
}


QString AppImage::getDescription()
{
    return m_description;
}

QString AppImage::getGithubLink()
{
    return m_links.value("github").toString();
}

QString AppImage::getInstallLink()
{
    return m_links.value("install").toString();
}

QString AppImage::getDownloadLink(QString architecture)
{
    return m_links.value("download_" + architecture, "").toString();
}

QVariantHash AppImage::links() const
{
    return m_links;
}

QVariantList AppImage::authors() const
{
    return m_authors;
}

QString AppImage::description() const
{
    return m_description;
}

QStringList AppImage::categories() const
{
    return m_categories;
}

QStringList AppImage::screenshots() const
{
    return m_screenshots;
}

QString AppImage::licence() const
{
    return m_licence;
}

void AppImage::setId(QString id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
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

void AppImage::setDescription(QString description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}

void AppImage::setCategories(QStringList categories)
{
    if (m_categories == categories)
        return;

    m_categories = categories;
    emit categoriesChanged(m_categories);
}

void AppImage::setScreenshots(QStringList screenshots)
{
    if (m_screenshots == screenshots)
        return;

    m_screenshots = screenshots;
    emit screenshotsChanged(m_screenshots);
}

void AppImage::setLicence(QString licence)
{
    if (m_licence == licence)
        return;

    m_licence = licence;
    emit licenceChanged(m_licence);
}
