#include "TasksListModel.h"

#include <QDebug>

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
        {ACTIONS, "actions"},
    };
}

int TasksListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return _tasks.length();
}

QVariant TasksListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const auto &row = index.row();
    if (row >= 0 && row < _tasks.length()) {
        const auto task = _tasks[row];

        switch (role) {
        case TASK_ID:
            return task.id;
        case STATUS:
            return task.status;
        case TITLE:
            return task.title;
        case SUBTITLE:
            return task.subTitle;
        case ICON:
            return task.iconPath;
        case CURRENT_PROGRESS:
            return task.current_progress;
        case TOTAL_PROGRESS:
            return task.total_progress;
        case ACTIONS:
            return getTackActionsVariantList(task);
        case APP_ID:
            return task.related_app_id;
        default:
            return {};
        }
    }

    return {};
}
QVariantList TasksListModel::getTackActionsVariantList(const TaskData &task)
{
    QVariantList actionsList;
    for (const auto &entry : task.actions)
        actionsList.push_back(entry.toVariant());

    return actionsList;
}
QHash<int, QByteArray> TasksListModel::roleNames() const
{
    return _roleNames;
}
void TasksListModel::handleTaskUpdate(const TaskData &notification)
{
    auto idx = taskIndex(notification);
    if (idx >= 0 && idx < _tasks.length()) {
        _tasks[idx] = notification;
        emit(dataChanged(index(idx, 0), index(idx, 0)));
    } else {
        beginInsertRows(QModelIndex(), idx, idx);
        _tasks.push_back(notification);
        endInsertRows();
    }
}

int TasksListModel::taskIndex(const TaskData &notification)
{
    int i = 0;
    const auto &tasksLength = _tasks.length();
    for (; i < tasksLength; i++) {
        if (_tasks[i].id == notification.id)
            return i;
    }

    return i;
}
