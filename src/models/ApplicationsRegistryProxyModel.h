#pragma once
// system

// libraries
#include <QSortFilterProxyModel>

// local
#include "ApplicationsRegistryModel.h"

class ApplicationsRegistryProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString sortRoleName READ getSortRoleName WRITE setSortRoleName NOTIFY sortRoleNameChanged);

public:
    explicit ApplicationsRegistryProxyModel(QObject *parent = nullptr);

public:
    Q_SLOT QString getSortRoleName() const;
    Q_SLOT void setSortRoleName(const QString &sortRoleName);

    Q_SIGNAL void sortRoleNameChanged(QString sortRoleName);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Q_SLOT void onSortRoleChanged(int sortRole);
};
