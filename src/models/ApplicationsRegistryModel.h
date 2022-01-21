#pragma once
// system

// libraries
#include <QAbstractItemModel>
#include <QModelIndex>
#include <services/update/ApplicationUpdateData.h>

// local
#include "services/ApplicationsRegistry.h"

class ApplicationsRegistryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ApplicationRoles {
        TypeRole = Qt::UserRole + 1,
        Id,
        Name,
        Version,
        Icon,
        Description,
        Snapshots,
        XdgCategories,
        Bundles,
        LatestBundlePath,
        LatestBundleSize,
        UpdateAvailable,
        Data,
    };

    Q_ENUM(ApplicationRoles)

    explicit ApplicationsRegistryModel(ApplicationsRegistry *registry, QObject *parent = nullptr);

    // Basic functionality:
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    Q_SLOT void handleApplicationAdded(const ApplicationData &application);
    Q_SLOT void handleApplicationUpdated(const ApplicationData &application);
    Q_SLOT void handleApplicationRemoved(const ApplicationData &application);
    Q_SLOT void handleUpdateInformation(const ApplicationUpdateData &updateInformation);

private:
    void initRoles();

    ApplicationsRegistry *_registry;
    ApplicationsList _applications;

    // app id -> update information map, map used to speed up queries
    QMap<QString, ApplicationUpdateData> _updatesAvailable;

    QModelIndex _root;
    QHash<int, QByteArray> _roles;
};
