#include "UpgradesListModel.h"

UpgradesListModel::UpgradesListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> UpgradesListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(CodeName, "app_codename");
    roles.insert(OldId, "old_app_id");
    roles.insert(NewId, "new_app_id");
    roles.insert(NewName, "new_app_name");
    roles.insert(NewDescription, "new_app_description");
    roles.insert(NewVersion, "new_app_version");
    roles.insert(NewChangeLog, "new_app_change_log");
    return roles;
}

int UpgradesListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return upgrades.size();
}

QVariant UpgradesListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || upgrades.length() <= index.row())
        return QVariant();

    const QMap<int, QVariant> &field = upgrades.at(index.row());
    return field.value(role);
}

void UpgradesListModel::setUpgrades(const QList<QMap<int, QVariant> > upgrades)
{
    beginResetModel();
    this->upgrades = upgrades;
    endResetModel();
}
