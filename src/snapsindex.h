#ifndef SNAPSINDEX_H
#define SNAPSINDEX_H

#include <QObject>
#include <QAbstractListModel>


class SnapD;
class SnapsIndex : public QAbstractListModel
{
    Q_OBJECT


public:
    SnapsIndex(QObject * parent = 0);
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex&) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex& index, int role) const ;

    Q_INVOKABLE void remove(QString snap);

private:
    void refreshCache();
    QVariantList cache;
    QStringList rolesCache;

    SnapD *snapd;
};

#endif // SNAPSINDEX_H
