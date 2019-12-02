#ifndef STOREMODEL_H
#define STOREMODEL_H

#ifdef STATIC_MAUIKIT
#include "fmh.h"
#include "mauilist.h"
#else
#include <MauiKit/fmh.h>
#include <MauiKit/mauilist.h>
#endif

class Store;
class Application;
class StoreModel : public MauiList
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap category READ getCategory WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QString categoryPath READ getCategoryPath NOTIFY categoryPathChanged)
    Q_PROPERTY(int page READ getPage WRITE setPage NOTIFY pageChanged)

public:
  StoreModel(QObject *parent = nullptr);

  // QQmlParserStatus interface
  void componentComplete();

  // MauiList interface
  FMH::MODEL_LIST items() const;

  QVariantMap getCategory() const;

  QString getCategoryPath() const;

  int getPage() const;

public slots:
  void setCategory(QVariantMap category);
  void clear();
  QVariantMap getApp(const QString &id);

  void setPage(int page);

signals:
  void categoryChanged(QVariantMap category);

  void categoryPathChanged(QString categoryPath);

  void pageChanged(int page);

private:
    FMH::MODEL_LIST m_list;
    QHash<QString, Application *> m_appMap; //id,app
    Store *m_store;
    QVariantMap m_category;
    QString m_categoryPath;
    int m_page = 0;

    //methods
    void requestApps();
};

#endif // STOREMODEL_H
