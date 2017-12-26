#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QException>
#include <QMap>
#include <QObject>

#include "Application.h"

class ApplicationNotFoundException : public QException {
};

class Repository : public QObject {
Q_OBJECT
    QMap<QString, Application> applications;

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
private:
};

#endif  // REPOSITORY_H
