#ifndef APPIMAGEREPOSITORY_H
#define APPIMAGEREPOSITORY_H

#include <QObject>
#include <QList>

class AppImage;

class AppImageRepository : public QObject
{
    Q_OBJECT
public:
    explicit AppImageRepository(QObject *parent = nullptr);

    Q_INVOKABLE virtual int count() = 0;
    Q_INVOKABLE virtual QList<AppImage *> list(int offset, int limit) = 0;
    Q_INVOKABLE virtual QList<AppImage *> search(QString query, int offset, int limit) = 0;

signals:

public slots:
};

#endif // APPIMAGEREPOSITORY_H
