#ifndef APPLICATIONVIEWCONTROLLER_H
#define APPLICATIONVIEWCONTROLLER_H

#include <QObject>
#include <QVariantMap>
#include <entities/Repository.h>
#include <entities/Registry.h>
#include <entities/Executor.h>
#include <gateways/RestClient.h>
#include <entities/AppImageInfo.h>
#include <gateways/GetApplicationRequest.h>

class ApplicationViewController : public QObject
{
    Q_OBJECT
    Registry *registry;
    Executor *executor;
    RestClient *restClient;
    GetApplicationRequest *request;

    AppImageInfo appImageInfo;
    ApplicationFull application;

    bool hasPendingTasks;
    Q_PROPERTY(QString backgroundImage READ getBackgroundImage NOTIFY applicationChanged)
    Q_PROPERTY(bool isAppInstalled READ isInstalled NOTIFY applicationChanged)
    Q_PROPERTY(bool hasPendingTasks MEMBER hasPendingTasks NOTIFY hasPendingTasksChanged)
    Q_PROPERTY(bool hasScreenShots READ hasScreenShots NOTIFY applicationChanged)

    Q_PROPERTY(QString appId READ getApplicationId NOTIFY applicationChanged)
    Q_PROPERTY(QString appIcon READ getApplicationIcon NOTIFY applicationChanged)
    Q_PROPERTY(QString appName READ getApplicationName NOTIFY applicationChanged)
    Q_PROPERTY(QString appAuthor READ getApplicationAuthor NOTIFY applicationChanged)
    Q_PROPERTY(QString appVersion READ getApplicationVersion NOTIFY applicationChanged)
    Q_PROPERTY(QString appDownloadSize READ getApplicationDownloadSize NOTIFY applicationChanged)
    Q_PROPERTY(QString appWebsite READ getApplicationWebsite NOTIFY applicationChanged)
    Q_PROPERTY(QString appDescription READ getApplicationDescription NOTIFY applicationChanged)
    Q_PROPERTY(QStringList appScreenShots READ getApplicationScreenShots NOTIFY applicationChanged)
public:
    explicit ApplicationViewController(QObject *parent = nullptr);

    void setRegistry(Registry* registry);
    void setExecutor(Executor *executor);
    void setExplorer(RestClient* explorer);

    QString getBackgroundImage();
    bool isInstalled();
    bool hasScreenShots();
    QString getApplicationId();
    QString getApplicationIcon();
    QString getApplicationName();
    QString getApplicationAuthor();
    QString getApplicationVersion();
    QString getApplicationDownloadSize();
    QString getApplicationWebsite();
    QString getApplicationDescription();
    QStringList getApplicationScreenShots();

signals:
    void applicationChanged();
    void hasPendingTasksChanged(bool hasPendingTasks);

public slots:
    void loadApplication(const QString &id);

protected slots:
    void handleTaskStarted(const QString &, const QVariantMap &data);

    void handleTaskCompleted(const QString &, const QVariantMap &data);
    void handleGetApplicationResult();

private:
    QString formatMemoryValue(float num);
    void checkIfHasPendingTasks();
    QVariantMap getLatestRelease() const;
};

#endif // APPLICATIONVIEWCONTROLLER_H
