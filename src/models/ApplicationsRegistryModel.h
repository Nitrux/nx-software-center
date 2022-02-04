#pragma once
// system

// libraries
#include <QAbstractItemModel>
#include <QModelIndex>
#include <services/TaskData.h>
#include <services/update/ApplicationUpdateData.h>

// local
#include "services/registry/ApplicationsRegistry.h"

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
        MainBundleIndex,
        MainBundlePath,
        MainBundleSize,
        UpdateAvailable,
        RelatedTask,
        Data,
    };

    Q_ENUM(ApplicationRoles)

    explicit ApplicationsRegistryModel(QObject *parent);
    void setRegistry(ApplicationsRegistry *registry);

    // Basic functionality:
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_SLOT void handleApplicationAdded(const Application &application);
    Q_SLOT void handleApplicationUpdated(const Application &application);
    Q_SLOT void handleApplicationRemoved(const Application &application);
    Q_SLOT void handleUpdateInformation(const ApplicationUpdateData &updateData);
    Q_SLOT void handleTaskUpdate(const TaskData &notification);

private:
    void initRoles();
    int findApplicationIndexById(const QString &);

    ApplicationsRegistry *_registry;
    ApplicationsList _applications;

    // app id -> update information map, map used to speed up queries
    QMap<QString, ApplicationUpdateData> _appliactionsUpdateData;

    // app id -> related tasks map, map used to speed up queries
    QMap<QString, TaskData> _applicationsRelatedTasks;

    QModelIndex _root;
    QHash<int, QByteArray> _roles;
    bool resolveUpdateAvailableValue(const Application &app) const;
    bool resolveRelatedTask(const Application &app) const;
    QStringList listBundlesFileNames(const Application &data) const;
};
