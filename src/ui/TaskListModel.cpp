#include "TaskListModel.h"

TaskListModel::TaskListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> TaskListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Id, "task_id");
    roles.insert(Description, "task_description");
    roles.insert(Status, "task_status");
    roles.insert(ProgressValue, "task_progress_value");
    roles.insert(ProgressTotal, "task_progress_total");
    roles.insert(ProgressMessage, "task_progress_message");
    return roles;
}

int TaskListModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    return tasks.size();
}



QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || tasks.size() <= index.row())
        return QVariant();

    QString id = tasksIds.at(index.row());
    QVariantMap t = tasks.value(id);

    // FIXME: Implement more
    QVariant ret;
    switch (role) {
    case Id:
        ret = id;
        break;
    case Description:
        ret = t.value("description");
        break;
    case Status:
        ret = t.value("status");
        break;
    case ProgressValue:
        ret = t.value("progress_value");
        break;
    case ProgressTotal:
        ret = t.value("progress_total");
        break;
    case ProgressMessage:
        ret = t.value("progress_message");
        break;
    default:
        break;
    }

    return ret;
}

void TaskListModel::addTask(const QString &id, const QVariantMap &data)
{
    int i =  tasksIds.length();
    beginInsertRows(QModelIndex(), i,i);

    tasksIds.append(id);
    tasks.insert(id, data);

    endInsertRows();
}

void TaskListModel::updateTask(const QString &id, const QVariantMap &data)
{
    int i = tasksIds.indexOf(id);
    QVariantMap newData = tasks[id];
    for (const QString &key: data.keys())
        newData.insert(key, data[key]);
    tasks[id] = newData;

    emit dataChanged(index(i, 0), index(i, 0));
}

void TaskListModel::removeTask(const QString &id)
{
    int i = tasksIds.indexOf(id);
    beginRemoveRows(index(-1,-1), i, i);

    tasksIds.removeAll(id);
    tasks.remove(id);

    endRemoveRows();
}
