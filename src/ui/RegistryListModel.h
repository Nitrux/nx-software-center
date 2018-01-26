#ifndef REGISTRYLISTMODEL_H
#define REGISTRYLISTMODEL_H

#include <QAbstractListModel>

class RegistryListModel : public QAbstractListModel
{
    Q_OBJECT

    QList<QVariantMap> records;

    enum ApplicationListModelRoles
    {
        Type = Qt::UserRole + 1,
        Message,
        TimeStamp,
        Persistence,
        Result,
        APP_ID,
        APP_NAME,
        APP_VERSION,
    };

public:
    explicit RegistryListModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setRecords(const QList<QVariantMap> &records);
private:
    QString createRecordMessage(const QVariantMap &record) const;
};

#endif // REGISTRYLISTMODEL_H
