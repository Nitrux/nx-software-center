#include "ApplicationData.h"
#include "Application.h"
#include "ApplicationBundle.h"

// libraries
#include <QDebug>
#include <QVersionNumber>

QString ApplicationData::getId() const
{
    return _data.value("xdg-id").toString();
}

void ApplicationData::setId(const QString &id)
{
    _data.insert("xdg-id", id);
}

QString ApplicationData::getVersion() const
{
    return _data.value("xdg-app-version").toString();
}

void ApplicationData::setVersion(const QString &version)
{
    _data.insert("xdg-app-version", version);
}

QString ApplicationData::getName() const
{
    return _data.value("xdg-name").toString();
}

void ApplicationData::setName(const QString &name)
{
    _data.insert("xdg-name", name);
}

QString ApplicationData::getIcon() const
{
    return _data.value("xdg-icon").toString();
}
void ApplicationData::setIcon(const QString &icon)
{
    _data.insert("xdg-icon", icon);
}

QString ApplicationData::getDescription() const
{
    return _data.value("xdg-description").toString();
}

void ApplicationData::setDescription(const QString &description)
{
    _data.insert("xdg-description", description);
}
QList<QUrl> ApplicationData::getSnapshots() const
{
    auto variantList = _data.value("xdg-snapshots").toList();

    QList<QUrl> results;
    for (const auto &entry : variantList)
        results.append(entry.toUrl());

    return results;
}

void ApplicationData::setSnapshots(const QList<QUrl> &snapshots)
{
    QVariantList variantList;
    for (const auto &item : snapshots)
        variantList.append(item);

    _data.insert("xdg-snapshots", variantList);
}

QStringList ApplicationData::getXdgCategories() const
{
    return _data.value("xdg-categories").toStringList();
}

void ApplicationData::setXdgCategories(const QStringList &xdgCategories)
{
    _data.insert("xdg-categories", xdgCategories);
}

void ApplicationData::setEntry(const QString &id, const QVariant &value)
{
    _data.insert(id, value);
}

void ApplicationData::setRequiresTerminal(bool requiresTerminal)
{
    _data.insert("xdg-terminal", requiresTerminal);
}
bool ApplicationData::requiresTerminal() const
{
    return _data.value("xdg-terminal", false).toBool();
}
QVariant ApplicationData::getEntry(const QString &id) const
{
    return _data.value(id);
}
QStringList ApplicationData::entries() const
{
    return _data.keys();
}
void ApplicationData::copy(const ApplicationData &data)
{
    const auto keys = data.entries();
    for (const auto &key : keys)
        _data.insert(key, data.getEntry(key));
}
