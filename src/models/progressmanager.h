#ifndef PROGRESSMANAGER_H
#define PROGRESSMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include "app.h"

class Package : public App
{
    Q_OBJECT
    Q_PROPERTY(QString link READ getLink NOTIFY linkChanged CONSTANT FINAL)
    Q_PROPERTY(QVariantMap package READ getPackage NOTIFY packageChanged CONSTANT FINAL)
    Q_PROPERTY(int percent MEMBER m_percent NOTIFY percentChanged)
    Q_PROPERTY(int packageIndex READ getPackageIndex NOTIFY packagedIndexChanged CONSTANT FINAL)
    using App::App;

private:
    int m_packageIndex;

    QString m_link;

    QVariantMap m_package;

public:
    Package(const Package &other, QObject *parent = nullptr);
    void stop();
    int m_percent;
    void setPackageIndex(const int &index);
    int getPackageIndex() const;
    QString getLink() const;
    QVariantMap getPackage() const;

signals:
    void percentChanged(int percent);
    void packagedIndexChanged(int packageIndex);
    void linkChanged(QString link);
    void packageChanged(QVariantMap package);
};

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
    void removePackage(App * app, const int &packageIndex);
    Package *takePackage(App * app, const int &packageIndex);
};



#endif // PROGRESSMANAGER_H
