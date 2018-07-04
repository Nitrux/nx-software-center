//
// Created by alexis on 7/3/18.
//

#ifndef NX_SOFTWARE_CENTER_APPIMAGEINFO_H
#define NX_SOFTWARE_CENTER_APPIMAGEINFO_H

#include <QMap>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QVariant>
#include <ostream>
#include "ApplicationFull.h"

class AppImageInfo : public ApplicationFull {
public:

    QString id;
    LocalizedQString name;
    QString icon;
    LocalizedQString abstract;
    LocalizedQString description;

    License license;
    QStringList categories;
    QStringList keywords;
    QStringList languages;
    Developer developer;
    Release release;
    File file;
    QList<RemoteImage> screenshots;
    QStringList mimeTypes;
    QMap<QString, QString> links;

    static AppImageInfo fromVariant(const QVariant& variant);
    static QVariant toVariant(const AppImageInfo& appImageInfo);

    bool operator==(const AppImageInfo& rhs) const;
    bool operator!=(const AppImageInfo& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const AppImageInfo& info);

};

#endif //NX_SOFTWARE_CENTER_APPIMAGEINFO_H
