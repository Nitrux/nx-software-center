#include "storemodel.h"
#include "appimagehubstore.h"
#include "app.h"

#include <QTimer>

StoreModel::StoreModel(QObject *parent) : MauiList(parent),
    m_store(new AppImageHubStore(this)), m_app(new Application(this)),  m_category(new Category(this))
    , m_applicationManager(new ApplicationManager())
  ,m_timer(new QTimer(this))
{
    qRegisterMetaType<Application *>("Application *");

    m_timer->setInterval(900);
    m_timer->setSingleShot(true);

    connect(m_timer, &QTimer::timeout, this, &StoreModel::requestApps);
}

void StoreModel::requestApps()
{
    if(!m_category)
        return;

    qDebug() << "@REQUEST APPS FOR ::::" << m_nameFilter << m_category->name << m_category->id << m_pageSize << m_page;

    if(m_category->id.isEmpty ())
    {
        return;
    }

    const auto categoryId = this->m_category->id == "0" ? "" : this->m_category->id;

    this->m_applicationManager->getApplications({categoryId}, this->m_nameFilter, static_cast<Store::SORT_MODE>(this->m_sort), QString::number(this->m_page), QString::number(this->m_pageSize), QStringList() << this->m_tags << "appimage", this->m_arch, m_category);
}

void StoreModel::componentComplete()
{
    connect(this->m_applicationManager, &ApplicationManager::applicationsResponseReady,
            [=](ApplicationResponseDTO *response) {
        for (const auto &app :response->applications)
        {
            //preview pics
            emit this->preItemAppended();

            m_appMap.insert(app->id, std::move(app));
            const auto preview_pic = app->previewPics.isEmpty() ? "" : app->previewPics.first()->pic;
            const auto small_pic = app->previewPics.isEmpty() ? "" : app->previewPics.first()->smallPic;
            this->m_list.append(FMH::MODEL{
                                    {FMH::MODEL_KEY::CHANGED, app->changed},
                                    {FMH::MODEL_KEY::COMMENTS, app->comments},
                                    {FMH::MODEL_KEY::CREATED, app->created},
                                    {FMH::MODEL_KEY::DESCRIPTION, app->description},
                                    {FMH::MODEL_KEY::DETAIL_PAGE, app->detailPage},
                                    {FMH::MODEL_KEY::DETAILS, app->details},
                                    {FMH::MODEL_KEY::GHNS_EXCLUDED, app->ghnsExcluded},
                                    {FMH::MODEL_KEY::ID, app->id},
                                    {FMH::MODEL_KEY::LANGUAGE, app->language},
                                    {FMH::MODEL_KEY::NAME, app->name},
                                    {FMH::MODEL_KEY::PERSON_ID, app->personId},
                                    {FMH::MODEL_KEY::PREVIEW, preview_pic},
                                    {FMH::MODEL_KEY::REPOSITORY, ""},
                                    {FMH::MODEL_KEY::SCORE, QString::number(app->score)},
                                    {FMH::MODEL_KEY::SMALL_PIC, small_pic},
                                    {FMH::MODEL_KEY::SUMMARY, app->summary},
                                    {FMH::MODEL_KEY::TAGS, app->tags},
                                    {FMH::MODEL_KEY::TOTAL_DOWNLOADS, QString::number(app->totalDownloads)},
                                    {FMH::MODEL_KEY::TYPE_ID, app->typeId},
                                    {FMH::MODEL_KEY::TYPE_NAME, app->typeName},
                                    {FMH::MODEL_KEY::VERSION, app->version},
                                    {FMH::MODEL_KEY::XDG_TYPE, app->xdgType}});
            emit this->postItemAppended();

        }

        emit countChanged();
    });

    connect(this, &StoreModel::pageChanged, this, [this](int)
    {
         m_timer->start();
    });

    connect(this, &StoreModel::pageSizeChanged, this, [this](int)
    {
         m_timer->start();
    });

    connect(this, &StoreModel::sortChanged, this, [this](StoreModel::SORT)
    {
         m_timer->start();
    });

    connect(this, &StoreModel::tagsChanged,  this, [this](QStringList)
    {
         m_timer->start();
    });

    connect(this, &StoreModel::nameFilterChanged,  this, [this](QString)
    {
         m_timer->start();
    });

//    connect(this, &StoreModel::categoryChanged,  this, [this](Category*)
//    {
//         m_timer->start();
//    });

//    requestApps();
    m_timer->start();
}

const FMH::MODEL_LIST &StoreModel::items() const
{
    return m_list;
}

Category * StoreModel::getCategory() const
{
    return m_category;
}

int StoreModel::getPage() const
{
    return m_page;
}

Application *StoreModel::getApp() const
{
    return m_app;
}

int StoreModel::getPageSize() const
{
    return m_pageSize;
}

StoreModel::SORT StoreModel::getSort() const
{
    return m_sort;
}

QStringList StoreModel::getTags() const
{
    return m_tags;
}

QString StoreModel::getNameFilter() const
{
    return m_nameFilter;
}

QString StoreModel::getCategoryName() const
{
    return m_category->name;
}

void StoreModel::setCategory(Category * category)
{
    if (m_category == category)
        return;

    m_category = category;

    qDebug() << "@@REQUEST APPS FOR CATEGORY" << m_category->name << m_category->id << m_pageSize;

    this->clear();
    this->setPage(0);
    emit categoryChanged(m_category);
    emit categoryNameChanged(m_category->name);
}

void StoreModel::clear()
{
    emit this->preListChanged();
    this->m_list.clear();
    emit this->postListChanged();
}

void StoreModel::setApp(const QString &id)
{
    const auto app = this->m_appMap[id];

    if(app)
        this->m_app = app;
    else
        this->m_app = new Application(this);

    emit this->appChanged(this->m_app);
}

Application * StoreModel::application(const QString & id)
{
    const auto app = this->m_appMap[id];

    if(app)
        return app;
    else
        return nullptr;
}

void StoreModel::setPage(int page)
{
//    if (m_page == page)
//        return;

    m_page = page;
    emit pageChanged(m_page);
}

void StoreModel::setPageSize(int pageSize)
{
    if (m_pageSize == pageSize)
        return;

    m_pageSize = pageSize;

    this->clear();
    emit pageSizeChanged(m_pageSize);
}

void StoreModel::setSort(StoreModel::SORT sort)
{
    if (m_sort == sort)
        return;

    m_sort = sort;

    this->clear();
    emit sortChanged(m_sort);
}

void StoreModel::setTags(QStringList tags)
{
    if (m_tags == tags)
        return;

    m_tags = tags;

    this->clear();
    emit tagsChanged(m_tags);
}

void StoreModel::setNameFilter(QString nameFilter)
{
    if (m_nameFilter == nameFilter)
        return;

    m_nameFilter = nameFilter;
    m_page = 0;

    this->clear();
    emit nameFilterChanged(m_nameFilter);
}


