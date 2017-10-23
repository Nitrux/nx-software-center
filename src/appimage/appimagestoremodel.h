#ifndef APPIMAGESTOREMODEL_H
#define APPIMAGESTOREMODEL_H

#include <QAbstractListModel>

#include "appimagehubrepository.h"

class AppImageStoreModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AppImageStoreModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    AppImageHubRepository m_appImageHubRepository;
private:
};

#endif // APPIMAGESTOREMODEL_H
