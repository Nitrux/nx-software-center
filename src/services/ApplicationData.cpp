#include "ApplicationData.h"
#include "ApplicationBundle.h"

#include <QDebug>
#include <QPixmap>
#include <QVariant>
#include <QVersionNumber>

ApplicationData::ApplicationData() = default;

ApplicationData::ApplicationData(ApplicationBundle *bundle)
{
    addBundle(bundle);
}

bool ApplicationData::operator==(const ApplicationData &rhs) const
{
    return getId() == rhs.getId();
}

bool ApplicationData::operator!=(const ApplicationData &rhs) const
{
    return !(rhs.getId() == this->getId());
}

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
QVariant ApplicationData::getEntry(const QString &id)
{
    return _data.value(id);
}
QList<QSharedPointer<ApplicationBundle>> ApplicationData::getBundles() const
{
    return _bundles;
}
void ApplicationData::setBundles(const QList<QSharedPointer<ApplicationBundle>> &bundles)
{
    _bundles = bundles;
}
void ApplicationData::addBundle(ApplicationBundle *bundle)
{
    this->addBundle(QSharedPointer<ApplicationBundle>(bundle));
}

void ApplicationData::addBundle(const QSharedPointer<ApplicationBundle> &bundle)
{
    int insertIndex = findInsertIndex(bundle->app->getVersion());
    _bundles.insert(insertIndex, bundle);

    if (!bundle->app.isNull()) {
        if (insertIndex == 0)
            copyBundleData(bundle);
    }
}

void ApplicationData::copyBundleData(const QSharedPointer<ApplicationBundle> &bundle)
{
    if (!bundle->app.isNull()) {
        const auto &new_data = bundle->app->_data;
        const auto keys = new_data.keys();
        for (const auto &key : keys)
            _data.insert(key, new_data.value(key));
    }
}

void ApplicationData::removeBundle(const QSharedPointer<ApplicationBundle> &bundle)
{
    if (_bundles.empty()) {
        qWarning() << "Trying to remove a bundle from an empty application";
        return;
    } else {
        int idx = _bundles.indexOf(bundle);
        if (idx == 0 && _bundles.length() > 1) {
            // the newest bundle is being removed we need to reset the data to the previous one
            copyBundleData(_bundles[1]);
        }

        _bundles.removeAt(idx);
    }
}

void ApplicationData::removeBundle(ApplicationBundle *bundle)
{
    removeBundle(QSharedPointer<ApplicationBundle>(bundle));
}

int ApplicationData::findInsertIndex(const QString &version)
{
    if (_bundles.length() == 0)
        return 0;
    else {
        int idx = 0;
        auto versionNumber = QVersionNumber::fromString(version);

        for (const auto &bundle : _bundles) {
            auto bundleVersionNumber = QVersionNumber::fromString(bundle->app->getVersion());
            if (versionNumber > bundleVersionNumber)
                return idx;

            idx++;
        }

        return idx;
    }
}
