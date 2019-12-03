#include "storemodel.h"
#include "appimagehubstore.h"
#include "app.h"

StoreModel::StoreModel(QObject *parent) : MauiList(parent),
    m_store(new AppImageHubStore(this)), m_app(new Application(this)),  m_category(new Category(this))
{}
void StoreModel::requestApps()
{
    if(!m_category)
        return;

#if defined Q_PROCESSOR_ARM && defined Q_OS_LINUX
    this->m_store->getApplicationsByArch({FMH::mapValue(this->m_category, FMH::MODEL_KEY::ID)}, "", Store::SORT_MODE::MODE_NEWEST, QString::number(this->m_page), QString::number(5), {}, Store::Arch::arm64);
#else
    this->m_store->getApplicationsByArch({this->m_category->id}, "", Store::SORT_MODE::MODE_NEWEST, QString::number(this->m_page), QString::number(5), {}, Store::Arch::amd64 );
#endif
}

void StoreModel::setCategoryPath()
{
    if(!m_category->isAParent())
        this->m_categoryPath = this->m_category->name;

    emit this->categoryPathChanged(this->m_categoryPath);
}
void StoreModel::componentComplete()
{
    connect(this->m_store, &Store::applicationsResponseReady,
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

    });
}

FMH::MODEL_LIST StoreModel::items() const
{
    return m_list;
}

Category * StoreModel::getCategory() const
{
    return m_category;
}

QString StoreModel::getCategoryPath() const
{
    return m_categoryPath;
}

int StoreModel::getPage() const
{
    return m_page;
}

Application *StoreModel::getApp() const
{
    return m_app;
}

void StoreModel::setCategory(Category * category)
{
    if (m_category == category)
        return;

    m_category = category;
    this->setCategoryPath();
    this->clear();
    this->setPage(0);
    emit categoryChanged(m_category);
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

void StoreModel::setPage(int page)
{
    if (m_page == page)
        return;

    m_page = page;
    this->requestApps();
    emit pageChanged(m_page);
}


