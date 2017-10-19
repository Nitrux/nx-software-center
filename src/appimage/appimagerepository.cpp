#include "appimagerepository.h"

#include "appimage.h"

AppImageRepository::AppImageRepository(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<AppImageList>("AppImageList");
}

QList<AppImage *> AppImageRepository::items() const
{
    return m_items;
}

void AppImageRepository::setItems(QList<AppImage *> items)
{
    if (m_items == items)
        return;

    m_items = items;
    emit itemsChanged(m_items);
}
