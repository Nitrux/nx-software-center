#ifndef SNAPDHELPER_H
#define SNAPDHELPER_H

#include <QByteArray>
#include <QObject>

#include <KAuth>

#include <Snapd/Client>

using namespace KAuth;


class SnapdHelper : public QObject
{
    Q_OBJECT
public:
    explicit SnapdHelper(QObject *parent = 0);

signals:

public slots:
    ActionReply remove(QVariantMap args);
    ActionReply install(QVariantMap args);
    ActionReply disable(QVariantMap args);
    ActionReply enable(QVariantMap args);

private:
    QSnapdClient m_qsnapdClient;
};

#endif // SNAPDHELPER_H
