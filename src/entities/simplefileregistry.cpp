#include "simplefileregistry.h"

SimpleFileRegistry::SimpleFileRegistry(): Registry()
{

}

bool SimpleFileRegistry::isReleaseDownloaded(QString appId, QString releaseId)
{
    return false;
}

QString SimpleFileRegistry::getReleaseFilePath(QString appId, QString releaseId)
{
    return "";
}
