#ifndef SNAPSINDEX_H
#define SNAPSINDEX_H

#include <QObject>
#include <QAbstractListModel>


class SnapsIndex : public QAbstractListModel
{
    Q_OBJECT


public:
    SnapsIndex(QObject *parent = 0);
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex&) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;

private:
    void refreshCache();
    QVariantList cache;
    QStringList rolesCache;

};

#endif // SNAPSINDEX_H
