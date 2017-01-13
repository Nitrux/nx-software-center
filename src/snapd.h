#ifndef SNAPD_H
#define SNAPD_H

#include <QMap>
#include <QObject>
#include <QVariant>

#include <kauthexecutejob.h>

#include "httputils.h"
#include "changes.h"
#include "snapstore.h"
#include "localsnaps.h"

class SnapD : public QObject
{
    Q_OBJECT

public:
    explicit SnapD(QObject *parent = 0);

    static SnapD* instance();

    Changes* changes() const;
    LocalSnaps* local() const;
    SnapStore* store() const;

public slots:
    QVariantList snaps();
    QVariantList find(QString query);

    Q_INVOKABLE KAuth::ExecuteJob *remove(QString snap);
    Q_INVOKABLE KAuth::ExecuteJob *install(QString snap);

signals:
    void changesChanged(Changes* changes);
    void storeChanged(SnapStore* store);
    void localChanged(LocalSnaps* local);

private:
    QByteArray localQuery(QByteArray query);
    HTTPUtils httpUtils;


    static SnapD *singleton;
};

#endif // SNAPD_H
