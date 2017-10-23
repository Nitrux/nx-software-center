#include "appimagestoremodel.h"

AppImageStoreModel::AppImageStoreModel(QObject *parent)
    : QAbstractListModel(parent), m_appImageHubRepository("http://localhost:4000/feed.json")
{
}

int AppImageStoreModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return 0;
}

QVariant AppImageStoreModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
