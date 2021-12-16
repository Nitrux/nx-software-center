#include "ApplicationsRegistryProxyModel.h"

#include <QMetaEnum>

ApplicationsRegistryProxyModel::ApplicationsRegistryProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    connect(this, &ApplicationsRegistryProxyModel::sortRoleChanged, this, &ApplicationsRegistryProxyModel::onSortRoleChanged);
}

QString ApplicationsRegistryProxyModel::getSortRoleName() const
{
    const auto &roleNames = sourceModel()->roleNames();
    const auto &roleName = roleNames[sortRole()];
    return roleName;
}
void ApplicationsRegistryProxyModel::setSortRoleName(const QString &sortRoleName)
{
    const auto &roleNames = sourceModel()->roleNames();
    const auto &roleNamesKeys = roleNames.keys();

    for (const auto &key : roleNamesKeys) {
        if (roleNames[key] == sortRoleName)
            setSortRole(key);
    }

    setSortRole(Qt::UserRole);
}
void ApplicationsRegistryProxyModel::onSortRoleChanged(int)
{
    sort(0, Qt::AscendingOrder);

    const auto &sortRoleName = getSortRoleName();
    emit(sortRoleNameChanged(sortRoleName));
}
