#ifndef APPLICATIONVIEWCONTROLLER_H
#define APPLICATIONVIEWCONTROLLER_H

#include <QObject>
#include <entities/Repository.h>
#include <entities/Registry.h>
#include <entities/Executor.h>

class ApplicationViewController : public QObject
{
    Q_OBJECT
    Repository *repository;
    Registry *registry;
    Executor *executor;

    Application application;

    bool hasPendingTasks;
    Q_PROPERTY(QString backgroundImage READ getBackgroundImage NOTIFY applicationChanged)
    Q_PROPERTY(bool isAppInstalled READ isInstalled NOTIFY applicationChanged)
    Q_PROPERTY(bool hasPendingTasks MEMBER hasPendingTasks NOTIFY hasPendingTasksChanged)

    Q_PROPERTY(QString appId READ getApplicationId NOTIFY applicationChanged)
    Q_PROPERTY(QString appIcon READ getApplicationIcon NOTIFY applicationChanged)
    Q_PROPERTY(QString appName READ getApplicationName NOTIFY applicationChanged)
    Q_PROPERTY(QString appAuthor READ getApplicationAuthor NOTIFY applicationChanged)
    Q_PROPERTY(QString appVersion READ getApplicationVersion NOTIFY applicationChanged)
    Q_PROPERTY(QString appDownloadSize READ getApplicationDownloadSize NOTIFY applicationChanged)
    Q_PROPERTY(QString appWebsite READ getApplicationWebsite NOTIFY applicationChanged)
    Q_PROPERTY(QString appDescription READ getApplicationDescription NOTIFY applicationChanged)
public:
    explicit ApplicationViewController(QObject *parent = nullptr);

    void setRepository(Repository *repository);
    void setRegistry(Registry *registry);
    void setExecutor(Executor *executor);

    QString getBackgroundImage();
    bool isInstalled();
    QString getApplicationId();
    QString getApplicationIcon();
    QString getApplicationName();
    QString getApplicationAuthor();
    QString getApplicationVersion();
    QString getApplicationDownloadSize();
    QString getApplicationWebsite();
    QString getApplicationDescription();
signals:
    void applicationChanged();
    void hasPendingTasksChanged(bool hasPendingTasks);

public slots:
    void loadApplication(const QString &id);

protected slots:
    void handleTaskStarted(const QString &, const QVariantMap &data);

    void handleTaskCompleted(const QString &, const QVariantMap &data);

private:

    QString formatMemoryValue(float num);
    void checkIfHasPendingTasks();
};

#endif // APPLICATIONVIEWCONTROLLER_H
