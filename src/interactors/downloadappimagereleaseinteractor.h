#ifndef DOWNLOADAPPIMAGERELEASEINTERACTOR_H
#define DOWNLOADAPPIMAGERELEASEINTERACTOR_H

#include <QList>
#include <QString>

#include "interactor.h"

class Registry;
class Repository;
class DownloadManager;
class DownloadAppImageReleaseInteractorListener;

class DownloadAppImageReleaseInteractor : public Interactor
{
public:
    DownloadAppImageReleaseInteractor(QString appImageId, QString appImageReleaseId,
                                      QList<Repository *> repositories,
                                      DownloadManager *downloadManager,
                                      Registry *registry,
                                      DownloadAppImageReleaseInteractorListener *listener);

    virtual ~DownloadAppImageReleaseInteractor();

    virtual void execute();

    QString storagePath = "$HOME/.local/bin/";
protected:
    QString m_appImageId;
    QString m_appImageReleaseId;
    QList<Repository *> m_repositories;
    DownloadManager *m_downloadManager;
    Registry *m_registry;
    DownloadAppImageReleaseInteractorListener *m_listener;
};

#endif // DOWNLOADAPPIMAGERELEASEINTERACTOR_H
