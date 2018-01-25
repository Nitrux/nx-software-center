//
// Created by alexis on 24/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_CACHE_H
#define NOMAD_SOFTWARE_CENTER_CACHE_H

#include <QSet>
#include <QString>
#include <QObject>

class Repository;

class Cache : public QObject {
Q_OBJECT
    Repository *repository;
    QSet<QString> applicationsInCache;
public:
    explicit Cache(QObject *parent = nullptr);

    void setRepository(Repository *repository);

    QStringList getApplicationIdsInCache();

    static const QString getApplicationsCachePath();

public slots:

    void handleInstalledApplicationsChanged(const QStringList &applicationsIds);

    void storeApplication(QString appId);

    void removeApplication(QString appId);

    void loadApplicationIdsInCache();
};


#endif //NOMAD_SOFTWARE_CENTER_CACHE_H
