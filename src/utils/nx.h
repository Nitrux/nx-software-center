#ifndef NX_H
#define NX_H
#include <QString>
#include <QUrl>
#include <MauiKit/fmh.h>

namespace NX
{

static const QString appName = QStringLiteral("NX Software Center");
static const QString displayName = QStringLiteral("NX Software Center");
static const QString version = QStringLiteral("1.0.0");
static const QString description = QStringLiteral("Software store");
static const QString orgName = QStringLiteral("Maui");
static const QString orgDomain = QStringLiteral("org.maui.nxsoftwarecenter");

static const QUrl AppsPath = FMH::HomePath+QStringLiteral("/Applications");

}


#endif // NX_H
