#ifndef TASKSLISTMODEL_H
#define TASKSLISTMODEL_H

#include <QAbstractListModel>

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
        APP_ID,
    };

public:
    explicit TasksListModel(QObject *parent = nullptr);

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_SLOT void handleTaskUpdate(const QVariantMap &taskUpdate);

private:
    QHash<int, QByteArray> _roleNames;
    QVariantList _tasks;
};

#endif // TASKSLISTMODEL_H
