#include "ApplicationData.h"
#include "ApplicationBundle.h"

#include <QDebug>
#include <QVariant>
#include <QVersionNumber>

ApplicationData::ApplicationData() = default;

ApplicationData::ApplicationData(QVariantMap data)
    : _data(std::move(data)){};

ApplicationData::ApplicationData(const ApplicationBundle &bundle)
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
QList<ApplicationBundle> ApplicationData::getBundles() const
{
    return _bundles;
}
void ApplicationData::setBundles(QList<ApplicationBundle> bundles)
{
    _bundles = bundles;
}
void ApplicationData::addBundle(const ApplicationBundle &bundle)
{
    int insertIndex = findInsertIndex(bundle.app->getVersion());
    // replace bundle if the paths match
    if (_bundles.length() > 0 && _bundles.length() < insertIndex && bundle.path == _bundles[insertIndex].path)
        _bundles.removeAt(insertIndex);

    _bundles.insert(insertIndex, bundle);

    if (bundle.app && insertIndex == 0)
        copyApplicationData(bundle);

    // adjust main bundle index if required
    auto mainBundleIndex = getMainBundleIndex();
    if (mainBundleIndex >= insertIndex || mainBundleIndex != 0)
        setMainBundleIndex(mainBundleIndex + 1);
}

void ApplicationData::copyApplicationData(const ApplicationBundle &bundle)
{
    if (bundle.app) {
        const auto &new_data = bundle.app->_data;
        const auto keys = new_data.keys();
        for (const auto &key : keys)
            _data.insert(key, new_data.value(key));
    } else {
        qWarning() << "Coping data from an incomplete bundle" << bundle.path;
    }
}

void ApplicationData::removeBundle(const ApplicationBundle &bundle)
{
    if (_bundles.empty()) {
        qWarning() << "Trying to remove a bundle from an empty application";
        return;
    } else {
        int idx = _bundles.indexOf(bundle);
        if (idx == 0 && _bundles.length() > 1) {
            // the newest bundle is being removed we need to reset the data to the previous one
            copyApplicationData(_bundles[1]);
        }

        _bundles.removeAt(idx);
    }
}

int ApplicationData::findInsertIndex(const QString &version)
{
    if (_bundles.length() == 0)
        return 0;
    else {
        int idx = 0;
        auto versionNumber = QVersionNumber::fromString(version);

        for (const auto &bundle : _bundles) {
            auto bundleVersionNumber = QVersionNumber::fromString(bundle.app->getVersion());
            if (versionNumber > bundleVersionNumber)
                return idx;

            idx++;
        }

        return idx;
    }
}

bool ApplicationData::requiresTerminal() const
{
    return _data.value("xdg-terminal").toBool();
}
void ApplicationData::setRequiresTerminal(bool requiresTerminal)
{
    _data.insert("xdg-terminal", requiresTerminal);
}
void ApplicationData::setMainBundleIndex(int idx)
{
    _data.insert("nxsc-main-bundle", idx);
}
int ApplicationData::getMainBundleIndex() const
{
    return _data.value("nxsc-main-bundle", 0).toInt();
}
ApplicationBundle ApplicationData::getMainBundle() const
{
    int mainBundleIdx = getMainBundleIndex();
    if (mainBundleIdx >= 0 && mainBundleIdx < _bundles.length())
        return _bundles[getMainBundleIndex()];
    else
        return {};
}
