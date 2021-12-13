#ifndef NX_H
#define NX_H

#include <QString>
#include <QUrl>
#include <MauiKit/FileBrowsing/fmstatic.h>

namespace NX
{
static const QUrl AppsPath = FMStatic::HomePath+QStringLiteral("/Applications");
static const QString appDBName = "/apps.db";
}


#endif // NX_H
