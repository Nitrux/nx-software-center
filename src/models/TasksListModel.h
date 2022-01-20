#pragma once
// system

// libraries
#include <QAbstractListModel>

// local
#include <services/TaskData.h>

class TasksListModel : public QAbstractListModel
{
    Q_OBJECT

    enum TasksListRoles {
        TASK_ID = Qt::UserRole + 1,
        STATUS,
        TITLE,
        SUBTITLE,
        ICON,
        CURRENT_PROGRESS,
        TOTAL_PROGRESS,
        ACTIONS,
        APP_ID,
    };

public:
    explicit TasksListModel(QObject *parent = nullptr);

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    // Basic functionality:
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    Q_SLOT void handleTaskUpdate(const TaskData &notification);

private:
    QHash<int, QByteArray> _roleNames;
    QList<TaskData> _tasks;

    int taskIndex(const TaskData &notification);
    QVariantList getTackActionsVariantList(const TaskData &task) const;
};
