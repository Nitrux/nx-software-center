#ifndef SNAPSTORE_H
#define SNAPSTORE_H

#include "snapsindex.h"

class SnapStore : public SnapsIndex
{
public:
    SnapStore();

    Q_INVOKABLE virtual void refresh() Q_DECL_OVERRIDE;
};

#endif // SNAPSTORE_H
