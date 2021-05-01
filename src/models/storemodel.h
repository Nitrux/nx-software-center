#ifndef STOREMODEL_H
#define STOREMODEL_H

#include <MauiKit/Core/fmh.h>
#include <MauiKit/Core/mauilist.h>

#include "store.h"

class Category;
class Application;
class StoreModel : public MauiList
{
    Q_OBJECT
    Q_PROPERTY(Category * category READ getCategory WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QString categoryName READ getCategoryName NOTIFY categoryNameChanged)
    Q_PROPERTY(int page READ getPage WRITE setPage NOTIFY pageChanged)
    Q_PROPERTY(int pageSize READ getPageSize WRITE setPageSize NOTIFY pageSizeChanged)
    Q_PROPERTY(Application * app READ getApp NOTIFY appChanged)
    Q_PROPERTY(StoreModel::SORT sort READ getSort WRITE setSort NOTIFY sortChanged)
    Q_PROPERTY(QStringList tags READ getTags WRITE setTags NOTIFY tagsChanged)
    Q_PROPERTY(QString nameFilter READ getNameFilter WRITE setNameFilter NOTIFY nameFilterChanged)

public:
    enum SORT
    {
        NEWEST = Store::SORT_MODE::MODE_NEWEST,
        ALPHABETICAL = Store::SORT_MODE::MODE_ALPHABETICAL,
        HIGHEST_RATED = Store::SORT_MODE::MODE_HIGHEST_RATED,
        MOST_DOWNLOADED = Store::SORT_MODE::MODE_MOST_DOWNLOADED
    };
    Q_ENUM(SORT)

    StoreModel(QObject *parent = nullptr);

    // QQmlParserStatus interface
    void componentComplete() override final;

    // MauiList interface
    const FMH::MODEL_LIST &items() const override final;

    Category *getCategory() const;

    int getPage() const;

    Application * getApp() const;

    int getPageSize() const;

    StoreModel::SORT getSort() const;

    QStringList getTags() const;

    QString getNameFilter() const;

    QString getCategoryName() const;

public slots:
    void setCategory(Category *category);
    void clear();
    void setApp(const QString &id);
    Application * application(const QString &id);

    void setPage(int page);

    void setPageSize(int pageSize);
    void setSort(StoreModel::SORT sort);
    void setTags(QStringList tags);

    void setNameFilter(QString nameFilter);

signals:
    void categoryChanged(Category * category);

    void pageChanged(int page);

    void appChanged(Application * app);

    void pageSizeChanged(int pageSize);

    void sortChanged(StoreModel::SORT sort);

    void tagsChanged(QStringList tags);

    void nameFilterChanged(QString nameFilter);

    void categoryNameChanged(QString categoryName);

private:
    FMH::MODEL_LIST m_list;
    QHash<QString, Application *> m_appMap; //id,app
    Store *m_store;
    Application * m_app;

    Category * m_category;

    int m_page = -1;
    int m_pageSize = 10;
    QString m_nameFilter;
    QStringList m_tags;
    SORT m_sort = SORT::NEWEST;    

#if defined Q_PROCESSOR_ARM && defined Q_OS_LINUX
    Store::Arch m_arch = Store::Arch::arm64;
#else
   Store::Arch m_arch = Store::Arch::amd64;
#endif
    //methods
    void requestApps();
};

#endif // STOREMODEL_H
