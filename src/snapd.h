#ifndef SNAPD_H
#define SNAPD_H

#include <QObject>
#include <QVariant>

#include "httputils.h"

class SnapD : public QObject
{
    Q_OBJECT
public:
    explicit SnapD(QObject *parent = 0);

    static SnapD* instance();
signals:

public slots:
    QVariantList snaps();
    QVariantList find(QString query);

    Q_INVOKABLE void remove(QString snap);
    Q_INVOKABLE void install(QString snap);

private:
    QByteArray localQuery(QByteArray query);
    HTTPUtils httpUtils;
    static SnapD *singleton;
};

#endif // SNAPD_H
