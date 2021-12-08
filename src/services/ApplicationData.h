#pragma once
// system

// libraries
#include <QMap>
#include <QObject>
#include <QPixmap>
#include <QUrl>
#include <QVariant>

// local
#include "ApplicationBundle.h"

/**
 * Contains the information about an Application and the binary bundles related to it.
 */
class ApplicationData
{
public:
    ApplicationData();
    ApplicationData(ApplicationBundle *bundle);

    QString getId() const;
    void setId(const QString &id);

    QString getVersion() const;
    void setVersion(const QString &version);

    QString getName() const;
    void setName(const QString &name);

    QString getIcon() const;
    void setIcon(const QString &icon);

    QString getDescription() const;
    void setDescription(const QString &description);

    QList<QUrl> getSnapshots() const;
    void setSnapshots(const QList<QUrl> &snapshots);

    QStringList getXdgCategories() const;
    void setXdgCategories(const QStringList &xdgCategories);

    QList<QSharedPointer<ApplicationBundle>> getBundles() const;
    void setBundles(const QList<QSharedPointer<ApplicationBundle>> &bundles);

    // add the bundle to the list and updates the current data if the bundle as a higher version
    void addBundle(ApplicationBundle *bundle);
    void addBundle(const QSharedPointer<ApplicationBundle> &bundle);

    // copies application data from bundle
    void copyBundleData(const QSharedPointer<ApplicationBundle> &bundle);

    void setEntry(const QString &id, const QVariant &value);
    QVariant getEntry(const QString &id);

    bool operator==(const ApplicationData &rhs) const;
    bool operator!=(const ApplicationData &rhs) const;

    // removes a bundle from the list and updates the current data if the bundle is the one with the higher version
    void removeBundle(ApplicationBundle *bundle);
    void removeBundle(const QSharedPointer<ApplicationBundle> &bundle);

private:
    // find the insert index for a new bundle with <version>
    int findInsertIndex(const QString &version);

    QVariantMap _data;

    QList<QSharedPointer<ApplicationBundle>> _bundles;
};
