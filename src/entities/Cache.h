//
// Created by alexis on 24/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_CACHE_H
#define NOMAD_SOFTWARE_CENTER_CACHE_H

#include <QSet>
#include <QString>

class Repository;
class Cache : public QObject {
Q_OBJECT
    Repository *repository;
    QSet<QString> applicationsInCache;
public:
    explicit Cache(Repository *repository, QObject *parent = nullptr);

    static const QString getApplicationsCachePath();

public slots:

    void handleInstalledApplicationsChanged(const QStringList &applicationsIds);

    void storeApplication(const QString appId);

    void removeApplication(const QString appId);
};


#endif //NOMAD_SOFTWARE_CENTER_CACHE_H
