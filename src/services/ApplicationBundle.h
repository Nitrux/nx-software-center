#pragma once
// system

// libraries
#include <QDateTime>
#include <QObject>
#include <QSharedPointer>
#include <QString>

// local

class ApplicationData;
Q_DECLARE_OPAQUE_POINTER(ApplicationData);

class ApplicationBundle
{
public:
    enum Type { Unknown, AppImage };

    ApplicationBundle();
    explicit ApplicationBundle(QString path, ApplicationData *data = nullptr);
    bool operator==(const ApplicationBundle &rhs) const;
    bool operator!=(const ApplicationBundle &rhs) const;

    QString path;
    QDateTime lastModified;
    QByteArray hashSumMD5;
    Type bundleType;

    QSharedPointer<ApplicationData> app;
};
