#ifndef SNAPSINDEX_H
#define SNAPSINDEX_H

#include <QObject>
#include <QAbstractListModel>


class SnapD;
class SnapsIndex : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString textFilter READ textFilter WRITE setTextFilter NOTIFY textFilterChanged)

public:
    SnapsIndex(QObject * parent = 0);
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex&) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex& index, int role) const ;

    Q_INVOKABLE void remove(QString snap);

    Q_INVOKABLE virtual void refresh() = 0;

    QString textFilter() const
    {
        return m_textFilter;
    }

public slots:
    void setTextFilter(QString textFilter)
    {
        if (m_textFilter == textFilter)
            return;

        m_textFilter = textFilter;
        emit textFilterChanged(textFilter);
    }

signals:
    void textFilterChanged(QString textFilter);

protected:
    QVariantList cache;
    QStringList rolesCache;

    SnapD *snapd;
    QString m_textFilter;
};

#endif // SNAPSINDEX_H
