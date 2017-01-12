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

    Q_PROPERTY(Changes* changes READ changes NOTIFY changesChanged)
    Q_PROPERTY(SnapStore* store READ store NOTIFY storeChanged)
    Q_PROPERTY(LocalSnaps* local READ local NOTIFY localChanged)

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


    Changes* m_changes;
    LocalSnaps* m_local;
    SnapStore* m_store;
};

#endif // SNAPD_H
