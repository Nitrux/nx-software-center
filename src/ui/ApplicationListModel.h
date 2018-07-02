#ifndef APPLICATIONLISTMODEL_H
#define APPLICATIONLISTMODEL_H

#include <QList>
#include <QAbstractListModel>

#include "entities/Application.h"

class ApplicationListModel : public QAbstractListModel
{
    Q_OBJECT
    enum ApplicationListModelRoles
    {
        Id = Qt::UserRole + 1,
        CodeName,
        Name,
        Version,
        Description,
        Icon
    };

public:
    explicit ApplicationListModel(QObject *parent = nullptr);

     QHash<int, QByteArray> roleNames() const;
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setApplications(const QList<QVariantMap> applications);
private:
    QList<QVariantMap> applications;
    QVariant getLocalizedValue(const QString& lcName, const QMap<QString, QVariant>& lcField) const;
};

#endif // APPLICATIONLISTMODEL_H
