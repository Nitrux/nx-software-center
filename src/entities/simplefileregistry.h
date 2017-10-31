#ifndef SIMPLEFILEREGISTRY_H
#define SIMPLEFILEREGISTRY_H

#include <QSettings>
#include "registry.h"

class SimpleFileRegistry : public Registry
{
public:
    SimpleFileRegistry();

    virtual void registerReleaseDownload(QString appId, QString releaseId, QString filePath) override;
    virtual void registerReleaseRemove(QString appId, QString releaseId) override;

private:
    void save();

};

#endif // SIMPLEFILEREGISTRY_H
