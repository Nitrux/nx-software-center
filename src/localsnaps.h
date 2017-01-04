#ifndef LOCALSNAPS_H
#define LOCALSNAPS_H

#include "snapsindex.h"

class LocalSnaps : public SnapsIndex
{
public:
    LocalSnaps();

    Q_INVOKABLE virtual void refresh() Q_DECL_OVERRIDE;

};

#endif // LOCALSNAPS_H
