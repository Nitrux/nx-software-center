#include "ApplicationsRegistryProxyModel.h"

#include <QDebug>

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

bool ApplicationsRegistryProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const auto &modelIndex = sourceModel()->index(source_row, 0, source_parent);

    const auto &name = sourceModel()->data(modelIndex, ApplicationsRegistryModel::Name).toString();
    const auto &description = sourceModel()->data(modelIndex, ApplicationsRegistryModel::Description).toString();

    auto regExp = filterRegExp();
    regExp.setCaseSensitivity(Qt::CaseInsensitive);

    return name.contains(regExp) || description.contains(regExp);
}
