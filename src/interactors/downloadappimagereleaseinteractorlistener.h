#ifndef DOWNLOADAPPIMAGERELEASEINTERACTORLISTENER_H
#define DOWNLOADAPPIMAGERELEASEINTERACTORLISTENER_H

#include "../entities/progresslistener.h"

class DownloadAppImageReleaseInteractorListener : public ProgressListener
{
public:
    virtual ~DownloadAppImageReleaseInteractorListener() {}

    virtual void downloadComplete(const QString filePaht) = 0;
};

#endif // DOWNLOADAPPIMAGERELEASEINTERACTORLISTENER_H
