#include "TasksListModel.h"

TasksListModel::TasksListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    _roleNames = {
        {TASK_ID, "task_id"},
        {STATUS, "status"},
        {APP_ID, "app_id"},
        {TITLE, "title"},
        {SUBTITLE, "subtitle"},
        {ICON, "icon"},
        {CURRENT_PROGRESS, "current_progress"},
        {TOTAL_PROGRESS, "total_progress"},
    };
}

int TasksListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 0;
}

QVariant TasksListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
QHash<int, QByteArray> TasksListModel::roleNames() const
{
    return _roleNames;
}
void TasksListModel::handleTaskUpdate(const QVariantMap &taskUpdate)
{
    int idx = -1;
    for (const auto &task : _tasks) {
    }
}
