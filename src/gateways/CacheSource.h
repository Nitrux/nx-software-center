//
// Created by alexis on 25/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_CACHESOURCE_H
#define NOMAD_SOFTWARE_CENTER_CACHESOURCE_H

#include <QDir>
#include <QList>

#include "entities/Application.h"
#include "entities/ApplicationSerializer.h"
#include "entities/Source.h"

class CacheSource : public Source {
Q_OBJECT
    const QString cachePath;
public:
    explicit CacheSource(const QString &cachePath, QObject *parent = nullptr);

    void fetchAllApplications() override;

private:
    QByteArray readFile(const QString &path) const;
};


#endif //NOMAD_SOFTWARE_CENTER_CACHESOURCE_H
