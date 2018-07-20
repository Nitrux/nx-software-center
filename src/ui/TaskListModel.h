#ifndef TASKLISTMODEL_H
#define TASKLISTMODEL_H

#include <QAbstractListModel>

class TaskListModel : public QAbstractListModel
{
    Q_OBJECT
    enum ApplicationListModelRoles
    {
        Id = Qt::UserRole + 1,
        Type,
        Description,
        Status,
        ProgressValue,
        ProgressTotal,
        ProgressMessage,
        AppId,
        AppName,
        AppAuthor,
        AppIcon,
    };
    QMap<QString, QVariantMap> tasks;
    QList<QString> tasksIds;

public:
    explicit TaskListModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const;
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addTask(const QString &id, const QVariantMap &data);
    void updateTask(const QString &id, const QVariantMap &data);
    void removeTask(const QString &id);
    QVariantMap getTask(const QString &id);
private:
};

#endif // TASKLISTMODEL_H
