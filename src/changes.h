#ifndef CHANGES_H
#define CHANGES_H

#include <QList>
#include <QVariantMap>
#include <QAbstractListModel>

class Changes : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit Changes(QObject *parent = 0);

    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void updateChanges(QVariantMap change);

private:
    int findIndex(QString changeId);
    QList<QVariantMap> m_changes;
    QStringList m_roles;
};

#endif // CHANGES_H
