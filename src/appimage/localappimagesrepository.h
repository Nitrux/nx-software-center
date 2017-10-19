#ifndef LOCALAPPIMAGESREPOSITORY_H
#define LOCALAPPIMAGESREPOSITORY_H

#include <QObject>

#include "appimagerepository.h"

class LocalAppImagesRepository : public AppImageRepository
{
public:
    LocalAppImagesRepository();

    Q_INVOKABLE virtual int count();
    Q_INVOKABLE virtual QList<AppImage *> list(int offset, int limit);
    Q_INVOKABLE virtual QList<AppImage *> search(QString query, int offset, int limit);

};

#endif // LOCALAPPIMAGESREPOSITORY_H
