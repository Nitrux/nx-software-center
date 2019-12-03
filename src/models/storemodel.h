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
class Category;
class Application;
class StoreModel : public MauiList
{
    Q_OBJECT
    Q_PROPERTY(Category * category READ getCategory WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QString categoryPath READ getCategoryPath NOTIFY categoryPathChanged)
    Q_PROPERTY(int page READ getPage WRITE setPage NOTIFY pageChanged)
    Q_PROPERTY(Application * app READ getApp NOTIFY appChanged)

public:
  StoreModel(QObject *parent = nullptr);

  // QQmlParserStatus interface
  void componentComplete();

  // MauiList interface
  FMH::MODEL_LIST items() const;

  Category *getCategory() const;

  QString getCategoryPath() const;

  int getPage() const;

  Application * getApp() const;

public slots:
  void setCategory(Category *category);
  void clear();
  void setApp(const QString &id);
  void setPage(int page);

signals:
  void categoryChanged(Category * category);

  void categoryPathChanged(QString categoryPath);

  void pageChanged(int page);

  void appChanged(Application * app);

private:
    FMH::MODEL_LIST m_list;
    QHash<QString, Application *> m_appMap; //id,app
    Store *m_store;
    Application * m_app;

    Category * m_category;
    QString m_categoryPath;
    int m_page = -1;

    //methods
    void requestApps();
    void setCategoryPath();
};

#endif // STOREMODEL_H
