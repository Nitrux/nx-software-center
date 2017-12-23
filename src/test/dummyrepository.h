#ifndef DUMMYREPOSITORY_H
#define DUMMYREPOSITORY_H

#include "Repository.h"

class DummyRepository : public Repository {
public:
  DummyRepository();

  virtual QString id();

  virtual void updateCache();
  QString appName = "Dummy App";
};

#endif // DUMMYREPOSITORY_H
