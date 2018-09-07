#ifndef APPLICATIONLISTMODEL_H
#define APPLICATIONLISTMODEL_H

#include <QList>
#include <QAbstractListModel>

#include <entities/ApplicationAbstract.h>
class ApplicationListModel : public QAbstractListModel {
Q_OBJECT
    enum ApplicationListModelRoles {
      Id = Qt::UserRole+1,
      Name,
      Version,
      Abstract,
      Icon,
      Size
    };

public:
    explicit ApplicationListModel(QObject* parent = nullptr);

    QHash<int, QByteArray> roleNames() const;
    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void setApplications(QList<ApplicationAbstract> applications);
private:
    QList<ApplicationAbstract> applications;
};

#endif // APPLICATIONLISTMODEL_H
