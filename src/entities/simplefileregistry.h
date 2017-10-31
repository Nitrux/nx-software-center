#ifndef SIMPLEFILEREGISTRY_H
#define SIMPLEFILEREGISTRY_H

#include "registry.h"

class SimpleFileRegistry : public Registry
{
public:
    SimpleFileRegistry();

    bool isReleaseDownloaded(QString appId, QString releaseId);
    QString getReleaseFilePath(QString appId, QString releaseId);
};

#endif // SIMPLEFILEREGISTRY_H
