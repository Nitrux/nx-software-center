#ifndef PROGRESSMANAGER_H
#define PROGRESSMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include "app.h"

class Downloader;
class Package : public App
{
    Q_OBJECT
    Q_PROPERTY(QString link READ getLink NOTIFY linkChanged CONSTANT FINAL)
    Q_PROPERTY(QVariantMap package READ getPackage NOTIFY packageChanged CONSTANT FINAL)
    Q_PROPERTY(MODE mode MEMBER m_mode NOTIFY modeChanged CONSTANT FINAL)
    Q_PROPERTY(QString modeLabel MEMBER m_modeLabel NOTIFY modeLabelChanged CONSTANT FINAL)
    Q_PROPERTY(int percent MEMBER m_percent NOTIFY percentChanged)
    Q_PROPERTY(int packageIndex READ getPackageIndex NOTIFY packagedIndexChanged CONSTANT FINAL)
    using App::App;

public:
    enum MODE : uint8_t
    {
        DOWNLOAD,
        LAUNCH,
        REMOVE,
        UPDATE,
        NONE
    };Q_ENUM(MODE)

    Package(const Package &other, QObject *parent = nullptr);
    void stop();
    void setPackageIndex(const int &index);
    void setMode(const Package::MODE &mode);
    int getPackageIndex() const;
    QString getModelLabel() const;
    QString getLink() const;
    QVariantMap getPackage() const;

private:
    QVariantMap m_package; //the actual package from the app to perform action upon
    int m_packageIndex; //the index of the actual package from the app

    QString m_link; //download link of the actual package
    int m_percent = 60; //percent from 0 to 100 on the current action(mode) being performed on the package
    Package::MODE m_mode = MODE::NONE;

    QString m_modeLabel = "Other";

signals:
    void percentChanged(int percent);
    void packagedIndexChanged(int packageIndex);
    void linkChanged(QString link);
    void packageChanged(QVariantMap package);
    void modeLabelChanged(QString modeLabel);
    void modeChanged(MODE mode);
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
    enum ROLES
    {
        ITEM,
        MODE
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
    Package * appendPackage(App * app, const int &packageIndex, const uint &mode);
    void removePackage(App * app, const int &packageIndex);
    Package *takePackage(App * app, const int &packageIndex);
};

#endif // PROGRESSMANAGER_H
