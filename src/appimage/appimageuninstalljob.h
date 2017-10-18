#ifndef APPIMAGEUNINSTALLJOB_H
#define APPIMAGEUNINSTALLJOB_H

#include <QObject>

#include <KJob>

class AppImageUninstallJob : public KJob
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
public:
    explicit AppImageUninstallJob(QString appImageId, QObject *parent = nullptr);

    virtual void start();
    QString status() const;

signals:

    void statusChanged(QString status);

public slots:

protected slots:
    void setStatus(QString status);

private:
    static void findRecursion(const QString &path, const QString &pattern, QStringList *result);

    QString m_appImageId;
    QString m_status;
};

#endif // APPIMAGEUNINSTALLJOB_H
