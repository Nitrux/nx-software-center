#include "appimagehubstore.h"

AppImageHubStore::AppImageHubStore(QObject *parent)
    : OpenDesktopStore(parent, "https://api.appimagehub.com/ocs/v1") {}

const QString AppImageHubStore::name() { return "AppImageHub"; }
