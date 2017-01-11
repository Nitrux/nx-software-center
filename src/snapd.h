#ifndef SNAPD_H
#define SNAPD_H

#include <QMap>
#include <QObject>
#include <QVariant>

#include <kauthexecutejob.h>

#include "httputils.h"
#include "changes.h"

class SnapD : public QObject
{
    Q_OBJECT
public:
    explicit SnapD(QObject *parent = 0);

    static SnapD* instance();


public slots:
    QVariantList snaps();
    QVariantList find(QString query);

    Q_INVOKABLE KAuth::ExecuteJob *remove(QString snap);
    Q_INVOKABLE KAuth::ExecuteJob *install(QString snap);

    Q_INVOKABLE Changes* changesModel();
private:
    QByteArray localQuery(QByteArray query);
    HTTPUtils httpUtils;
    Changes m_changes;

    static SnapD *singleton;


};

#endif // SNAPD_H
