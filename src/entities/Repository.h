#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QException>
#include <QMap>
#include <QObject>
#include <QReadWriteLock>

#include "Source.h"
#include "entities/Application.h"
#include "interactors/FetchApplicationsInteractor.h"

class ApplicationNotFoundException : public QException {
    QString appId;
public:
    ApplicationNotFoundException(const QString appId) : QException(), appId(appId) {}
    const QString getAppId() { return appId; }
};

class Repository : public QObject {
Q_OBJECT
    QMap<QString, Application> applications;

    bool isBeingUpdated;
    Q_PROPERTY(bool isBeingUpdated
                       READ getIsBeingUpdated
                       WRITE setIsBeingUpdated
                       NOTIFY
                       isBeingUpdateChanged)

    QReadWriteLock mutex;
public:
    explicit Repository(QObject *parent = nullptr);

    void add(Application app);

    bool contains(const QString &id);

    Application get(const QString &id);

    int countAll();

    int countByName();

    QList<Application> getAll();

    QList<Application> getAllLatestVersions();

    Application getLatestVersion(const QString &name);

    QList<Application> getAllVersions(const QString &name);

    QList<Application> filterAll(const QString &query);

    QList<Application> filterLatestsVersions(const QString &query);

    void remove(const QString &id);

    void removeAllVersions(const QString &name);

    void removeAll();

    void setIsBeingUpdated(bool isBeingUpdated);

    bool getIsBeingUpdated();

signals:

    void isBeingUpdateChanged(const bool &isBeingUpdated);

    void changed();

private:
};

#endif  // REPOSITORY_H
