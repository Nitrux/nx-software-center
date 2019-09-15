#ifndef STOREMODEL_H
#define STOREMODEL_H

#ifdef STATIC_MAUIKIT
#include "fmh.h"
#include "mauilist.h"
#else
#include <MauiKit/fmh.h>
#include <MauiKit/mauilist.h>
#endif


class StoreModel : public MauiList
{
public:
  StoreModel(QObject *parent = nullptr);
};

#endif // STOREMODEL_H
