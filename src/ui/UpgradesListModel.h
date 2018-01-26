#ifndef UPGRADESLISTMODEL_H
#define UPGRADESLISTMODEL_H

#include <QAbstractListModel>

class UpgradesListModel : public QAbstractListModel
{
    Q_OBJECT
    QList<QMap<int, QVariant>> upgrades;
public:
    enum ApplicationListModelRoles
    {
        CodeName = Qt::UserRole + 1,
        OldId,
        NewId,
        NewName,
        NewDescription,
        NewVersion,
        NewChangeLog,
    };

    explicit UpgradesListModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const;


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = NewName) const override;

    void setUpgrades(const QList<QMap<int, QVariant>> upgrades);
private:
};

#endif // UPGRADESLISTMODEL_H
