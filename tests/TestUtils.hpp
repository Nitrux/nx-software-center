#ifndef NX_SOFTWARE_CENTER_TEST_UTILS_H
#define NX_SOFTWARE_CENTER_TEST_UTILS_H

#include "services/ApplicationBundle.h"

ApplicationBundle mockApplicationBundle(const QString &appName, const QString &version)
{
    ApplicationBundle bundle;

    bundle.path = "/tmp/file/path/" + appName + "_v" + version + ".AppImage";
    bundle.size = 1024;
    bundle.lastModified = QDateTime::currentDateTime();
    bundle.hashSumMD5 = "x0x0x0";
    bundle.bundleType = ApplicationBundle::AppImage;
    bundle.data.setId(appName.toLower());
    bundle.data.setName("Demo App");
    bundle.data.setVersion(version);

    return bundle;
}

#endif // NX_SOFTWARE_CENTER_TEST_UTILS_H
