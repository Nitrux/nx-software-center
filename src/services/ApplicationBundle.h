#pragma once
// system

// libraries
#include <QDateTime>
#include <QObject>
#include <QSharedPointer>
#include <QString>

// local
#include "ApplicationData.h"

class ApplicationBundle
{
public:
    enum Type { Unknown, AppImage };

    ApplicationBundle();
    explicit ApplicationBundle(QString path, ApplicationData data = {});
    bool operator==(const ApplicationBundle &rhs) const;
    bool operator!=(const ApplicationBundle &rhs) const;
    bool operator<(const ApplicationBundle &rhs) const;
    bool operator>(const ApplicationBundle &rhs) const;

    QString path;
    qint64 size;
    QString version;
    QDateTime lastModified;
    QByteArray hashSumMD5;
    Type bundleType;

    ApplicationData data;
};

Q_DECLARE_METATYPE(ApplicationBundle)