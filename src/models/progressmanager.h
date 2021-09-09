#ifndef PROGRESSMANAGER_H
#define PROGRESSMANAGER_H

#include <QObject>
#include <QAbstractListModel>

#include "app.h"
#include "package.h"

class ProgressManager : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count MEMBER m_count NOTIFY countChanged)

private:
    QHash<QString, Package *> m_packages;
    QList<Package *> m_list;
    int m_count = 0;

    bool contains(const App &app, const int &packageIndex) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

public:
    enum ROLES
    {
        ITEM
    };
    explicit ProgressManager(QObject *parent = nullptr);
    ProgressManager(const App &app, QObject *parent = nullptr);
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;

signals:
    void countChanged(int count);
    void warning(QString message);
    void packageAppended(Package * package);
    void packagedRemoved(Package *package);
    void packagedFinished(Package *package);

public slots:
    Package * appendPackage(App * app, const int &packageIndex);
    void removePackage(const int &packageIndex);
    void stopPackage(const int &packageIndex);
    Package *takePackage(const int &packageIndex);
};

#endif // PROGRESSMANAGER_H
