#ifndef SNAPDHELPER_H
#define SNAPDHELPER_H

#include <KAuth>
#include <QByteArray>
#include <QObject>


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

private:
    QByteArray sendRequest(QByteArray request, ActionReply reply);
};

#endif // SNAPDHELPER_H
