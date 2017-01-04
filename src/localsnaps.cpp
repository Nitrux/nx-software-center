#include "localsnaps.h"

#include <QVariant>
#include <QString>
#include <QSet>

#include "snapd.h"

LocalSnaps::LocalSnaps()
{
    refresh();
}

void LocalSnaps::refresh()
{
    QVariantList oldCache = cache;

    beginResetModel();
    cache = snapd->snaps();

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
