#include "installappimagereleaseinteractor.h"

#include <appimage/appimage.h>

InstallAppImageReleaseInteractor::InstallAppImageReleaseInteractor(const QString &file, QObject *parent): QObject(parent)
{
    this->file = file;
}

void InstallAppImageReleaseInteractor::execute()
{
//    appimage_register_in_system(file.toLocal8Bit(), 0);
    emit finished();
}
