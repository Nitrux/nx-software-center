#ifndef APPSMODEL_H
#define APPSMODEL_H

#include <QObject>

#include <MauiKit/Core/mauilist.h>

#include <services/TaskManager.h>

class QFileSystemWatcher;

class Store;
class Application;
class AppsModel : public MauiList
{
    Q_OBJECT

    // Q_PROPERTY(bool isUpdatable MEMBER m_isUpdatable NOTIFY isUpdatableChanged)
    // Q_PROPERTY(TaskManager* taskManager MEMBER taskManager NOTIFY taskManagerChanged)
    Q_PROPERTY(QVector<bool> isAppUpdatable MEMBER m_isAppUpdatable NOTIFY isAppUpdatableChanged)

public:
    // bool m_isUpdatable = false;

    explicit AppsModel(QObject *parent = nullptr);

    // QQmlParserStatus interface
    void componentComplete() override final;

    const FMH::MODEL_LIST &items() const override final;

    void setAppUpdatable(int index);
    FMH::MODEL_LIST getItems();

signals:
    void appLaunchSuccess();
    void appLaunchError(int err);
    void appDeleteSuccess();
    void appUpdateSuccess(QString msg);
    void appUpdateError(QString msg);

    // void isUpdatableChanged(bool isUpdatable);
    void isAppUpdatableChanged();
    // void taskManagerChanged();

public slots:
    void launchApp(const int &index);
    void removeApp(const int &index);
    void resfresh();

private:
    FMH::MODEL_LIST m_list;
    QHash<QString, Application *> m_app;
    Store *m_store;
    QFileSystemWatcher * m_watcher;
    QAppImageUpdate *updater;
    // TaskManager *taskManager;
    Task *updateTask;
    QVector<bool> m_isAppUpdatable;

    void setList();

    void unintegrate(const QUrl &url);
    void clear();

    void handleError(short errorCode, short action);
    void handleFinished(QJsonObject info, short action);
};

#endif // APPSMODEL_H
