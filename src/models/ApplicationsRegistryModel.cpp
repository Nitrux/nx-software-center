#include "ApplicationsRegistryModel.h"

#include <QDebug>

ApplicationsRegistryModel::ApplicationsRegistryModel(ApplicationsRegistry *registry, QObject *parent)
    : QAbstractListModel(parent)
    , _registry(registry)
{
    _applications = registry->getApplications();

    connect(_registry, &ApplicationsRegistry::applicationAdded, this, &ApplicationsRegistryModel::handleApplicationAdded, Qt::QueuedConnection);
    connect(_registry, &ApplicationsRegistry::applicationUpdated, this, &ApplicationsRegistryModel::handleApplicationUpdated, Qt::QueuedConnection);
    connect(_registry, &ApplicationsRegistry::applicationRemoved, this, &ApplicationsRegistryModel::handleApplicationRemoved, Qt::QueuedConnection);

    initRoles();
}

QHash<int, QByteArray> ApplicationsRegistryModel::roleNames() const
{
    return _roles;
}

void ApplicationsRegistryModel::initRoles()
{
    _roles[Id] = "id";
    _roles[Name] = "name";
    _roles[Version] = "version";
    _roles[Icon] = "icon";
    _roles[Description] = "description";
    _roles[Snapshots] = "snapshots";
    _roles[XdgCategories] = "xdg_categories";
    _roles[Bundles] = "bundles";
    _roles[LatestBundlePath] = "latest_bundle_path";
}

int ApplicationsRegistryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        auto app = _applications[parent.row()];
        return app.getBundles().length();
    }

    return _applications.length();
}
QVariant ApplicationsRegistryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    if (index.row() >= 0 && index.row() < _applications.length()) {
        const auto &app = _applications.at(index.row());
        switch (role) {
        case Id:
            return app.getId();
        case Name:
            return app.getName();
        case Version:
            return app.getVersion();
        case Icon:
            return app.getIcon();
        case Description:
            return app.getDescription();
        case XdgCategories:
            return app.getXdgCategories();
        case LatestBundlePath:
            return app.getBundles().first().path;
        default:
            return {};
        }
    }
    return {};
}

void ApplicationsRegistryModel::handleApplicationAdded(const ApplicationData &application)
{
    int length = _applications.length();
    beginInsertRows(QModelIndex(), length, length);
    _applications.push_back(application);
    endInsertRows();
}

void ApplicationsRegistryModel::handleApplicationUpdated(const ApplicationData &application)
{
    auto row = _applications.indexOf(application);
    _applications[row] = application;

    if (row >= 0 && row < _applications.length())
        emit(dataChanged(index(row, 0), index(row + 1, 0)));
}

void ApplicationsRegistryModel::handleApplicationRemoved(const ApplicationData &application)
{
    auto idx = _applications.indexOf(application);

    beginRemoveRows(QModelIndex(), idx, idx);
    _applications.removeAt(idx);
    endRemoveRows();
}
