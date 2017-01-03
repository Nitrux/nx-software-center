#include "snapsindex.h"

#include <QVariant>
#include <QSet>
#include <QDebug>

#include "snapd.h"

SnapsIndex::SnapsIndex(QObject *parent) : QAbstractListModel(parent)
{
    snapd = SnapD::instance();
    refreshCache();
}

QHash<int, QByteArray> SnapsIndex::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    for (int i = 0; i < rolesCache.length(); i ++) {
        roles.insert(i + Qt::UserRole, rolesCache.at(i).toLocal8Bit());
    }

    qDebug() << roles;
    return roles;
}


int SnapsIndex::rowCount(const QModelIndex &) const {
    return cache.length();
}

QVariant SnapsIndex::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= cache.length())
        return QVariant();

    QString propertyName;
    if (role >= Qt::UserRole)
        propertyName = rolesCache.at(role - Qt::UserRole);

    if (role == Qt::ToolTipRole)
        propertyName = "summary";

    propertyName.remove(0,1);
    propertyName.replace("_","-");

    QVariantMap item = cache.at(index.row()).toMap();
    return item.value(propertyName, QVariant());
}


void SnapsIndex::refreshCache() {
    QVariantList oldCache = cache;

    beginResetModel();
    cache = snapd->list();

    rolesCache.clear();
    for (QVariant item : cache) {
        QVariantMap map = item.toMap();
        rolesCache.append(map.keys());
    }

    rolesCache = QSet<QString>::fromList(rolesCache).toList();
    for (QString &roleName : rolesCache) {
        roleName.replace("-","_");
        roleName.prepend("_");
    }
    endResetModel();
}
