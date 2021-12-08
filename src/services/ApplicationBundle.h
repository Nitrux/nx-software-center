#pragma once
// system

// libraries
#include <QObject>
#include <QSharedPointer>
#include <QString>

// local

class ApplicationData;
class ApplicationBundle : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationBundle(const QString &path, ApplicationData *data = nullptr);
    bool operator==(const ApplicationBundle &rhs) const;
    bool operator!=(const ApplicationBundle &rhs) const;

    QString path;
    QSharedPointer<ApplicationData> app;
};
