#include "snapstore.h"

#include <QSet>
#include <QString>
#include "snapd.h"

SnapStore::SnapStore()
{

    connect(this, &SnapsIndex::textFilterChanged, this, &SnapStore::refresh);
}

void SnapStore::refresh()
{
    QVariantList oldCache = cache;

    beginResetModel();
    cache = snapd->find(m_textFilter);

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
