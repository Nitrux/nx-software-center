#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QObject>

#ifdef STATIC_MAUIKIT
#include "fmh.h"
#include "mauilist.h"
#else
#include <MauiKit/fmh.h>
#include <MauiKit/mauilist.h>
#endif

class Store;
class CategoriesModel : public MauiList
{
public:
    CategoriesModel(QObject * parent = nullptr);

    // QQmlParserStatus interface
    void componentComplete() override final;

    // MauiList interface
    FMH::MODEL_LIST items() const override final;

private:
    FMH::MODEL_LIST m_list;
    Store *m_store;
};

#endif // CATEGORIESMODEL_H
