#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QException>
#include <QMap>
#include <QObject>

#include "Source.h"
#include "entities/Application.h"
#include "interactors/FetchApplicationsInteractor.h"

class ApplicationNotFoundException : public QException {
};

class Repository : public QObject {
Q_OBJECT
    QMap<QString, Application> applications;

    bool isBeingUpdated;
    QList<Source*> sources;
    FetchApplicationsInteractor *fetchApplicationsInteractor;

public:
    explicit Repository(QObject *parent = nullptr);

    void add(Application app);

    bool contains(const QString &id) const;

    Application get(const QString &id) const;

    int countAll() const;

    int countByName() const;

    QList<Application> getAll() const;

    QList<Application> getAllLatestVersions() const;

    Application getLatestVersion(const QString &name) const;

    QList<Application> getAllVersions(const QString &name) const;

    QList<Application> filterAll(const QString &query) const;

    QList<Application> filterLatestsVersions(const QString &query) const;

    void remove(const QString &id);

    void removeAllVersions(const QString &name);

    void removeAll();

    void update();

    void setSources(const QList<Source*> &sources);

signals:
    void updateStarted();
    void updateComplete();
    void updateError();

protected slots:
    void handleUpdateResults();

private:
};

#endif  // REPOSITORY_H
