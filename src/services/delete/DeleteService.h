#pragma once
// system

// libraries
#include <QObject>

// local
#include <services/Application.h>

class DeleteService : public QObject
{
    Q_OBJECT
public:
    DeleteService(int appBundlesLimit, QObject *parent);
    Q_SCRIPTABLE void deleteApplication(const QVariant &variant);
    Q_SIGNAL void deleteError(const QVariant &variant, const QString &what);

    // keep up to <appBundlesLimit> bundles for a given application
    Q_SLOT void onApplicationUpdated(const Application &applicationData);

private:
    void removeApplicationBundle(const QVariant &variant, const ApplicationBundle &bundle);

    int _appBundlesLimit;
};
