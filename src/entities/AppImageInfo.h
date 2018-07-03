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

class AppImageInfo {
public:
    class LocalizedQString : public QMap<QString, QString> {
    public:
        friend std::ostream& operator<<(std::ostream& os, const LocalizedQString& string);

        static LocalizedQString fromVariant(const QVariant &variant);
        static QVariant toVariant(const LocalizedQString &string);
    };

    class License {
    public:
        QString id;
        QString name;
        QString body;

        bool operator==(const License& rhs) const;
        bool operator!=(const License& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const License& license);
        static License fromVariant(QVariant& variant);
        static QVariant toVariant(const License& license);
    };

    class Developer {
    public:
        QString name;
        QString avatar;
        QString website;
        QString pubkey;

        bool operator==(const Developer& rhs) const;
        bool operator!=(const Developer& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const Developer& developer);
        static Developer fromVariant(QVariant& variant);
        static QVariant toVariant(const Developer& developer);
    };

    class Release {
    public:
        QDateTime date;
        QString version;
        QString channel;
        LocalizedQString changelog;
        bool operator==(const Release& rhs) const;
        bool operator!=(const Release& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const Release& release);
        static Release fromVariant(const QVariant& variant);
        static QVariant toVariant(const Release& release);
    };

    class File {
    public:
        int type;
        int size;
        QString architecture;
        QString sha512checksum;
        QString remote_url;
        QString local_url;

        File();
        bool operator==(const File& rhs) const;
        bool operator!=(const File& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const File& file);
        static File fromVariant(const QVariant& variant);
        static QVariant toVariant(const File& file);
    };

    class RemoteImage {
    public:
        int height;
        int width;
        QString language;
        QString caption;
        QString url;

        RemoteImage();
        bool operator==(const RemoteImage& rhs) const;
        bool operator!=(const RemoteImage& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const RemoteImage& image);
        static RemoteImage fromVariant(const QVariant& variant);
        static QVariant toVariant(const RemoteImage& image);
    };

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
