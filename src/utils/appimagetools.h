#ifndef APPIMAGETOOLS_H
#define APPIMAGETOOLS_H

#include <QObject>
#include <QUrl>
#include <MauiKit/Core/fmh.h>

class AppImageTools : public QObject
{
    Q_OBJECT
public:
    explicit AppImageTools(QObject *parent = nullptr);

    static void integrate(const QUrl &url);
    static void unintegrate(const QUrl &url);
   static const FMH::MODEL desktopData(const QUrl &url);

signals:

};

#endif // APPIMAGETOOLS_H
