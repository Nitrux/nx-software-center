#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QMap>
#include <QList>
#include <QString>

class App;
class Release;

class Repository
{
public:
    Repository();
    virtual ~Repository();
    virtual QString id() = 0;

    virtual void updateCache() = 0;
    virtual QList<App*> list();
    virtual bool contains(QString appId);
    virtual bool contains(QString appId, QString releaseId);
    virtual App* getApp(QString appId);
    virtual Release* getRelease(QString appId, QString releaseId);
protected:
    QMap<QString, App*> apps;
};

#endif // REPOSITORY_H
