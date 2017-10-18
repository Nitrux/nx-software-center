#ifndef APPIMAGEINSTALLJOB_H
#define APPIMAGEINSTALLJOB_H

#include <QObject>

#include <KJob>

class AppImageInstallJob : public KJob
{
    Q_OBJECT
public:
    explicit AppImageInstallJob(QObject *parent = nullptr);

signals:

public slots:
};

#endif // APPIMAGEINSTALLJOB_H
