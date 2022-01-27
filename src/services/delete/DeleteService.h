#pragma once
// system

// libraries
#include <QObject>

// local

class DeleteService : public QObject
{
    Q_OBJECT
public:
    Q_SCRIPTABLE void deleteApplication(const QVariant &variant);

    Q_SIGNAL void deleteError(const QVariant &variant, const QString &what);
};
