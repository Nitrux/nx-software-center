#include "localappimagesrepository.h"

LocalAppImagesRepository::LocalAppImagesRepository()
{

}

int LocalAppImagesRepository::count()
{
    return 0;
}

QList<AppImage *> LocalAppImagesRepository::list(int offset, int limit)
{
    return QList<AppImage *>();
}

QList<AppImage *> LocalAppImagesRepository::search(QString query, int offset, int limit)
{
    return QList<AppImage *>();
}
