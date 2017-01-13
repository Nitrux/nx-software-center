#include "changes.h"

#include <QJsonDocument>
#include <QTimer>
#include <QDebug>

Changes::Changes(QObject *parent)
    : QAbstractListModel(parent)
{
   //  m_roles << "id" << "kind"  << "ready" << "ready-time"  << "spawn-time"  << "status"  << "summary"  <<  "tasks";
}

QHash<int, QByteArray> Changes::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    for (int i = 0; i < m_roles.length(); i ++) {
        roles.insert(i + Qt::UserRole, m_roles.at(i).toLocal8Bit());
    }

    return roles;
}

int Changes::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    qDebug() << m_changes.size();
    return m_changes.size();
}

QVariant Changes::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_changes.size())
        return QVariant();

    QString role_name = m_roles.at(role - Qt::UserRole);
    QVariant value = m_changes.at(index.row()).value(role_name);

    return value;
}

void Changes::updateChanges(QVariantMap change)
{
    beginResetModel();
    QString id = change.value("id").toString();
    if (id.isEmpty())
        return;

    int idx = findIndex(id);
    if (idx == -1) {
        qDebug() << "Inserting change" << id;
//        beginInsertRows(createIndex(0,0), m_changes.size(), m_changes.size());
        m_changes.append(change);
//        endInsertRows();
    }else {
        qDebug() << "Updating change" << id;
        m_changes.replace(idx, change);
//        emit(dataChanged(createIndex(idx, 0), createIndex(idx, 0)));
    }

//    qDebug() << "Available keys: " << change.keys();

    if (change.value("ready").toBool()){
        // Schedule for removal
        qDebug() << "Scheduling for removal change" << id;
        QTimer::singleShot(3000, [this,idx, id] () {
            qDebug() << "Removing change" << id;
            beginResetModel();
            m_changes.removeAt(idx);
            endResetModel();
        });

        return;
    }
    endResetModel();
}

int Changes::findIndex(QString changeId)
{
    for (int i = 0; i < m_changes.size(); i ++) {
        QString id = m_changes.at(i).value("id").toString();
        if (id.compare(changeId) == 0)
            return i;
    }

    return -1;
}
