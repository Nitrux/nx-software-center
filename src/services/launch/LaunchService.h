#pragma once
// system

// libraries
#include <QObject>
#include <QProcess>
#include <services/ApplicationBundle.h>

// local

class LaunchService : public QObject
{
    Q_OBJECT
public:
    Q_SCRIPTABLE void launch(const QVariant &application, int bundleIdx = -1) const;
    Q_SCRIPTABLE QVariantList listInstalledBundles(const QVariant &applicationVariant);

    Q_SIGNAL void launchError(const QVariant &application, const QString &what) const;

private:
    QProcess *createCLIProcess(const ApplicationBundle &bundle) const;
    QProcess *createGUIProcess(const ApplicationBundle &bundle) const;
};
