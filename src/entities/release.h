#ifndef RELEASE_H
#define RELEASE_H

#include <QMap>
#include <QList>
#include <QString>

class Change;

class Release
{
public:
    Release(QString id, QString app_id);
    virtual ~Release();

    QString id;
    QString app_id;
    QString version;
    QString arch;

    QString name;
    QString description;

    QList<QString> authors;

    QString licence;

    int download_size = 0;
    QString download_link;
    QString icon_link;
    QList<QString> screenshot_links;

    virtual Change* download() = 0;
    virtual Change* install() = 0;
    virtual Change* uninstall() = 0;
    virtual Change* remove() = 0;

    bool operator == (const Release& r) const;
    bool operator < (const Release& r) const;

    int Compare (const Release& r) const;
};

#endif // RELEASE_H
