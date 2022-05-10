#pragma once

#include <QObject>
#include <QString>
#include <QUrl>

#include "apprepoversiondto.h"
#include "apprepogroupresponsedto.h"

class AppRepoPackageResponseDTO {
    public:
        AppRepoPackageResponseDTO();

        void setName(QString name);
        QString getName();
        void setSlug(QString slug);
        QString getSlug();
        void setVersion(QString version);
        QString getVersion();
        void setDescription(QString description);
        QString getDescription();
        void setHash(QString hash);
        QString getHash();
        void setPackage(QString package);
        QString getPackage();
        void setIcon(QUrl icon);
        QUrl getIcon();
        void setPreview(QUrl preview);
        QUrl getPreview();
        void setFile(QUrl file);
        QUrl getFile();
        void setVersions(QList<AppRepoVersionDTO> versions);
        QList<AppRepoVersionDTO> getVersions();
        void setGroups(QList<AppRepoGroupResponseDTO> groups);
        QList<AppRepoGroupResponseDTO> getGroups();
        void setImages(QStringList images);
        QStringList getImages();
        
    private:
        QString _name;
        QString _slug;
        QString _version;
        QString _description;
        QString _hash;
        QString _package;
        QUrl _icon;
        QUrl _preview;
        QUrl _file;
        QList<AppRepoVersionDTO> _versions;
        QList<AppRepoGroupResponseDTO> _groups;
        QStringList _images;
};