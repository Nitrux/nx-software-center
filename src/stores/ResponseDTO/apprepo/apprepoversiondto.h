#pragma once

#include <QObject>
#include <QString>
#include <QUrl>

class AppRepoVersionDTO {
    public:
        AppRepoVersionDTO();

        void setName(QString name);
        QString getName();
        void setDescription(QString description);
        QString getDescription();
        void setHash(QString hash);
        QString getHash();
        void setFile(QUrl file);
        QUrl getFile();
        void setIpfsCid(QString ipfsCid);
        QString getIpfsCid();
        void setIpfsGateway(QString ipfsGateway);
        QString getIpfsGateway();

    private:
        QString _name;
        QString _description;
        QString _hash;
        QUrl _file;
        QString _ipfsCid;
        QString _ipfsGateway;
};