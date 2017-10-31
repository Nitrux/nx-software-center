#ifndef TASKSLISTMODEL_H
#define TASKSLISTMODEL_H

#include <QAbstractListModel>

class TasksListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TasksListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // TASKSLISTMODEL_H