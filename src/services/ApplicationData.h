#pragma once
// system

// libraries
#include <QMap>
#include <QObject>
#include <QUrl>
#include <QVariant>

// local
#include "ApplicationBundle.h"

typedef QVector<ApplicationData> ApplicationsList;

/**
 * Contains the information about an Application and the binary bundles related to it.
 */
class ApplicationData
{
public:
    ApplicationData();
    explicit ApplicationData(QVariantMap data);
    explicit ApplicationData(const ApplicationBundle &bundle);

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

    bool requiresTerminal() const;
    void setRequiresTerminal(bool requiresTerminal);

    QList<QUrl> getSnapshots() const;
    void setSnapshots(const QList<QUrl> &snapshots);

    QStringList getXdgCategories() const;
    void setXdgCategories(const QStringList &xdgCategories);

    QList<ApplicationBundle> getBundles() const;
    void setBundles(QList<ApplicationBundle> bundles);

    // add the bundle to the list and updates the current data if the bundle as a higher version
    void addBundle(const ApplicationBundle &bundle);

    // removes a bundle from the list and updates the current data if the bundle is the one with the higher version
    void removeBundle(const ApplicationBundle &bundle);

    void setEntry(const QString &id, const QVariant &value);
    QVariant getEntry(const QString &id);

    // define the main bundle for this application
    void setMainBundleIndex(int idx);
    int getMainBundleIndex() const;
    ApplicationBundle getMainBundle() const;


    bool operator==(const ApplicationData &rhs) const;
    bool operator!=(const ApplicationData &rhs) const;

private:
    // copies application data from bundle
    void copyApplicationData(const ApplicationBundle &bundle);

    // find the insert index for a new bundle with <version>
    int findInsertIndex(const QString &version);

    QVariantMap _data;

    QList<ApplicationBundle> _bundles;
};

Q_DECLARE_METATYPE(ApplicationData)
Q_DECLARE_METATYPE(ApplicationsList)