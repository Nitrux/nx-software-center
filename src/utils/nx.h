#ifndef NX_H
#define NX_H

#include <QString>
#include <QUrl>
#include <MauiKit3/FileBrowsing/fmstatic.h>

namespace NX
{
static const QUrl AppsPath = FMStatic::HomePath+QStringLiteral("/Applications");
static const QString appDBName = "cache.db";
}


#endif // NX_H
