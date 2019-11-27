#ifndef STORES_APPIMAGEHUBSTORE_H
#define STORES_APPIMAGEHUBSTORE_H

#include "opendesktopstore.h"

class AppImageHubStore : public OpenDesktopStore {
public:
  AppImageHubStore(QObject *parent = nullptr);

  const QString name() override final;
};

#endif
