#include "TaskListModel.h"

TaskListModel::TaskListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> TaskListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Id, "task_id");
    roles.insert(Type, "task_type");
    roles.insert(Description, "task_description");
    roles.insert(Status, "task_status");
    roles.insert(ProgressValue, "task_progress_value");
    roles.insert(ProgressTotal, "task_progress_total");
    roles.insert(ProgressMessage, "task_progress_message");

    roles.insert(AppId, "task_application_id");
    roles.insert(AppName, "task_application_name");
    roles.insert(AppAuthor, "task_application_author");
    return roles;
}

int TaskListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return tasksIds.size();;
}



QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || tasksIds.size() <= index.row())
        return QVariant();

    QString id = tasksIds.at(index.row());
    QVariantMap t = tasks.value(id);

    // FIXME: Implement more
    QVariant ret;
    switch (role) {
    case Id:
        ret = id;
        break;
    case Type:
        ret = t.value("type");
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
    case AppId:
        ret = t.value("task_application_id");
        break;
    case AppName:
        ret = t.value("task_application_name");
        break;
    case AppAuthor:
        ret = t.value("task_application_author");
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
    if (i != -1) {
        QVariantMap newData = tasks[id];
        for (const QString &key: data.keys())
            newData.insert(key, data[key]);
        tasks[id] = newData;


        emit dataChanged(index(i, 0), index(i, 0));
    }
}

void TaskListModel::removeTask(const QString &id)
{
    int i = tasksIds.indexOf(id);
    beginRemoveRows(index(-1,-1), i, i);

    tasks.remove(id);
    tasksIds.removeAll(id);

    endRemoveRows();
}

QVariantMap TaskListModel::getTask(const QString &id)
{
    return tasks.value(id);
}
