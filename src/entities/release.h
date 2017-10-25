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
    QString arch;

    QString name;
    QString description;

    std::list<QString> authors;

    QString licence;

    QString download_link;
    QString icon_link;
    QList<QString> screenshot_links;

    virtual Change* download() = 0;
    virtual Change* install() = 0;
    virtual Change* uninstall() = 0;
    virtual Change* remove() = 0;

    bool operator == (const Release& r) const;
    bool operator < (const Release& r) const;

    int Compare (const Release& d) const;
};

#endif // RELEASE_H
