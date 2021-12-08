#pragma once
// system

// libraries
#include <QObject>
#include <QSharedPointer>
#include <QString>

// local

class ApplicationData;
Q_DECLARE_OPAQUE_POINTER(ApplicationData);

class ApplicationBundle
{
public:
    ApplicationBundle();
    explicit ApplicationBundle(QString path, ApplicationData *data = nullptr);
    bool operator==(const ApplicationBundle &rhs) const;
    bool operator!=(const ApplicationBundle &rhs) const;

    QString path;
    QSharedPointer<ApplicationData> app;
};
