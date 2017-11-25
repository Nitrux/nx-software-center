#ifndef INSTALLAPPIMAGERELEASEINTERACTOR_H
#define INSTALLAPPIMAGERELEASEINTERACTOR_H

#include <QObject>

class InstallAppImageReleaseInteractor : public QObject
{
    Q_OBJECT
    QString file;
public:
    explicit InstallAppImageReleaseInteractor(
            const QString &file,
            QObject *parent = nullptr);

signals:
    void finished();
    void error(const QString &message);

public slots:
    virtual void execute();
};

#endif // INSTALLAPPIMAGERELEASEINTERACTOR_H
