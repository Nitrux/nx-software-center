//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_APPLICATIONFULL1_H
#define NX_SOFTWARE_CENTER_APPLICATIONFULL1_H

#include <ostream>
#include <QString>
#include <QVariant>
#include <QDateTime>

class ApplicationFull {
public:
    class LocalizedQString : public QMap<QString, QString> {
    public:
        static LocalizedQString fromVariant(const QVariant& variant);
        static QVariant toVariant(const LocalizedQString& string);

        friend std::ostream& operator<<(std::ostream& os, const LocalizedQString& string);
    };
    class License {
    public:
        QString id;
        QString name;
        QString body;

        bool operator==(const License& rhs) const;
        bool operator!=(const License& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const License& license);
        static License fromVariant(const QVariant& variant);
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
        static Developer fromVariant(const QVariant& variant);
        static QVariant toVariant(const Developer& developer);
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

    class Release {
    public:
        QDateTime date;
        QString version;
        QString channel;
        LocalizedQString changelog;
        QList<File> files;
        bool operator==(const Release& rhs) const;
        bool operator!=(const Release& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const Release& release);
        static Release fromVariant(const QVariant& variant);
        static QVariant toVariant(const Release& release);
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
    QList<Release> releases;
    QList<RemoteImage> screenshots;
    QStringList mimeTypes;
    QMap<QString, QString> links;
};

#endif //NX_SOFTWARE_CENTER_APPLICATIONFULL1_H
