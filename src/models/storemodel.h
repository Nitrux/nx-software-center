#ifndef STOREMODEL_H
#define STOREMODEL_H

#include <MauiKit/Core/fmh.h>
#include <MauiKit/Core/mauilist.h>

#include "store.h"
#include "../services/storemanager/storemanagerservice.h"

class QTimer;
class Category;
class PlingStoreApplicationData;
class StoreModel : public MauiList
{
    Q_OBJECT
    Q_PROPERTY(Category * category READ getCategory WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QString categoryName READ getCategoryName NOTIFY categoryNameChanged)
    Q_PROPERTY(int page READ getPage WRITE setPage NOTIFY pageChanged)
    Q_PROPERTY(int pageSize READ getPageSize WRITE setPageSize NOTIFY pageSizeChanged)
    Q_PROPERTY(PlingStoreApplicationData * app READ getApp NOTIFY appChanged)
    Q_PROPERTY(StoreModel::SORT sort READ getSort WRITE setSort NOTIFY sortChanged)
    Q_PROPERTY(QStringList tags READ getTags WRITE setTags NOTIFY tagsChanged)
    Q_PROPERTY(QString nameFilter READ getNameFilter WRITE setNameFilter NOTIFY nameFilterChanged)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)

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

    PlingStoreApplicationData * getApp() const;

    int getPageSize() const;

    StoreModel::SORT getSort() const;

    QStringList getTags() const;

    QString getNameFilter() const;

    QString getCategoryName() const;

    QString user() const;

public slots:
    void setCategory(Category *category);
    void clear();
    void setApp(const QString &id);
    PlingStoreApplicationData * application(const QString &id);

    void setPage(int page);

    void setPageSize(int pageSize);
    void setSort(StoreModel::SORT sort);
    void setTags(QStringList tags);

    void setNameFilter(QString nameFilter);

    void setUser(QString user);

signals:
    void categoryChanged(Category * category);

    void pageChanged(int page);

    void appChanged(PlingStoreApplicationData * app);

    void pageSizeChanged(int pageSize);

    void sortChanged(StoreModel::SORT sort);

    void tagsChanged(QStringList tags);

    void nameFilterChanged(QString nameFilter);

    void categoryNameChanged(QString categoryName);

    void userChanged(QString user);

private:
    FMH::MODEL_LIST m_list;
    QHash<QString, PlingStoreApplicationData *> m_appMap; //id,app
    PlingStoreApplicationData * m_app;

    Category * m_category;

    int m_page = -1;
    int m_pageSize = 10;
    QString m_nameFilter;
    QStringList m_tags;
    SORT m_sort = SORT::NEWEST;    

    StoreManagerService *m_storeManagerService;
    QTimer * m_timer;
#if defined Q_PROCESSOR_ARM && defined Q_OS_LINUX
    Store::Arch m_arch = Store::Arch::arm64;
#else
   Store::Arch m_arch = Store::Arch::amd64;
#endif
    //methods
   void requestApps();
   QString m_user;
};

#endif // STOREMODEL_H
