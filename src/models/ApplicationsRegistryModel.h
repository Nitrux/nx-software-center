#pragma once
// system

// libraries
#include <QAbstractItemModel>
#include <QModelIndex>

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
    };

    explicit ApplicationsRegistryModel(ApplicationsRegistry *registry, QObject *parent = nullptr);

    // Basic functionality:
    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_SLOT void handleApplicationAdded(const ApplicationData &application);
    Q_SLOT void handleApplicationUpdated(const ApplicationData &application);
    Q_SLOT void handleApplicationRemoved(const ApplicationData &application);

private:
    void initRoles();

    ApplicationsRegistry *_registry;
    QList<ApplicationData> _applications;

    QModelIndex _root;
    QHash<int, QByteArray> _roles;
};
