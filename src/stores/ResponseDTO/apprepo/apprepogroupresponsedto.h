#pragma once

#include <QObject>
#include <QString>
#include <QUrl>

class AppRepoGroupResponseDTO {
    public:
        AppRepoGroupResponseDTO();

        void setName(QString name);
        QString getName();
        void setDescription(QString description);
        QString getDescription();
        void setIcon(QUrl icon);
        QUrl getIcon();
        void setUnique(int unique);
        int getUnique();

    private:
        QString _name;
        QString _description;
        QUrl _icon;
        int _unique;
};