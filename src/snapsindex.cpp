#include "snapsindex.h"

#include <QDebug>

SnapsIndex::SnapsIndex(QObject *parent) : QAbstractListModel(parent)
{

}

QHash<int, QByteArray> SnapsIndex::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Qt::DisplayRole, "display");
    roles.insert(Qt::DecorationRole, "decoration");


    for (int i = Qt::UserRole; i < rolesCache.length(); i ++) {
        roles.insert(i, rolesCache.at(i - Qt::UserRole).toLocal8Bit());
    }

    return roles;
}




int SnapsIndex::rowCount(const QModelIndex &) const {
    return cache.length();
}


QVariant SnapsIndex::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() > cache.length())
        return QVariant();

    QString propertyName = rolesCache.at(role - Qt::UserRole);
    return cache.at(index.row()).value<QVariantMap>().value(propertyName);
}

void SnapsIndex::refreshCache() {
    QVariantList oldCache = cache;
    // TODO: fetch installed snaps and its info
}
