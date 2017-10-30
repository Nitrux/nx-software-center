#ifndef APP_H
#define APP_H

#include <QMap>
#include <QList>
#include <QString>

class Release;

class App
{
public:
    App(QString id);

    QString id;


    Release* lastRelease();
    void addRelease(Release* release);
    Release* getRelease(QString releaseId);
    QList<Release*> releases();
    QList<QString> categories;

    QMap<QString, QString> links;

protected:
    QMap<QString, Release *> m_releases;
};

#endif // APP_H
