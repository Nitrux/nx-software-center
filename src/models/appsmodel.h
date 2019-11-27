#ifndef APPSMODEL_H
#define APPSMODEL_H

#include <QObject>

#include "../stores/store.h"

#ifdef STATIC_MAUIKIT
#include "fmh.h"
#include "mauilist.h"
#else
#include <MauiKit/fmh.h>
#include <MauiKit/mauilist.h>
#endif

class AppsModel : public MauiList {
  Q_OBJECT
  Q_PROPERTY(QString categoryUri READ getCategoryUri WRITE setCategoryUri NOTIFY
                 categoryUriChanged)

public:
  explicit AppsModel(QObject *parent = nullptr);
  FMH::MODEL_LIST items() const override final;

  QString getCategoryUri() const;
  void setCategoryUri(QString categoryUri);

signals:
  void categoryUriChanged(QString categoryUri);

private:
  FMH::MODEL_LIST m_list;
  Store *appImageHubStore;

  void setList();

  QString m_categoryUri;
};

#endif // APPSMODEL_H
