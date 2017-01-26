#ifndef SNAPDCLIENTKAUTHWRAPPER_H
#define SNAPDCLIENTKAUTHWRAPPER_H

#include <QObject>
#include <KAuth/KAuthExecuteJob>

class SnapdClientKAuthWrapper : public QObject
{
    Q_OBJECT
public:
    SnapdClientKAuthWrapper(QObject *parent = 0);

    Q_INVOKABLE KAuth::ExecuteJob *disable(const QString &snap);
    Q_INVOKABLE KAuth::ExecuteJob *enable(const QString &snap);
    Q_INVOKABLE KAuth::ExecuteJob *remove(const QString &snap);
    Q_INVOKABLE KAuth::ExecuteJob *install(const QString &snap, const QString &channel);
    Q_INVOKABLE KAuth::ExecuteJob *refresh(const QString &snap, const QString &channel);
};

#endif // SNAPDCLIENTKAUTHWRAPPER_H
