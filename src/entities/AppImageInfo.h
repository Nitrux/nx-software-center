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
#include "Application.h"

class AppImageInfo {
public:

    QString id;
    Application::LocalizedQString name;
    QString icon;
    Application::LocalizedQString abstract;
    Application::LocalizedQString description;

    Application::License license;
    QStringList categories;
    QStringList keywords;
    QStringList languages;
    Application::Developer developer;
    Application::Release release;
    Application::File file;
    QList<Application::RemoteImage> screenshots;
    QStringList mimeTypes;
    QMap<QString, QString> links;

    static AppImageInfo fromVariant(const QVariant &variant);

    static QVariant toVariant(const AppImageInfo &appImageInfo);

    QVariant toVariant() const;

    bool operator==(const AppImageInfo &rhs) const;

    bool operator!=(const AppImageInfo &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const AppImageInfo &info);

};

#endif //NX_SOFTWARE_CENTER_APPIMAGEINFO_H
