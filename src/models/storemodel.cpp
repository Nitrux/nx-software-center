#include "storemodel.h"
#include "appimagehubstore.h"

StoreModel::StoreModel(QObject *parent) : MauiList(parent),
    m_store(new AppImageHubStore(this))
{

}
void StoreModel::requestApps()
{
#if defined Q_PROCESSOR_ARM && defined Q_OS_LINUX
    this->m_store->getApplicationsByArch({FMH::mapValue(this->m_category, FMH::MODEL_KEY::ID)}, "", Store::SORT_MODE::MODE_NEWEST, QString::number(this->m_page), QString::number(5), {}, Store::Arch::arm64);
#else
    this->m_store->getApplicationsByArch({FMH::mapValue(this->m_category, FMH::MODEL_KEY::ID)}, "", Store::SORT_MODE::MODE_NEWEST, QString::number(this->m_page), QString::number(5), {}, Store::Arch::amd64 );
#endif
}
void StoreModel::componentComplete()
{
    connect(this->m_store, &Store::applicationsResponseReady,
            [=](ApplicationResponseDTO *response) {
        for (const auto &app :response->applications)
        {
            //preview pics
            emit this->preItemAppended();

            m_appMap.insert(app->id, app);
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
                                    {FMH::MODEL_KEY::GPG_FINGERPRINT, ""},
                                    {FMH::MODEL_KEY::GPG_SIGNATURE, ""},
                                    {FMH::MODEL_KEY::ID, app->id},
                                    {FMH::MODEL_KEY::LANGUAGE, app->language},
                                    {FMH::MODEL_KEY::NAME, app->name},
                                    {FMH::MODEL_KEY::PACKAGE_TYPE, ""},
                                    {FMH::MODEL_KEY::PERSON_ID, app->personId},
                                    {FMH::MODEL_KEY::PIC, ""},
                                    {FMH::MODEL_KEY::PREVIEW, preview_pic},
                                    {FMH::MODEL_KEY::REPOSITORY, ""},
                                    {FMH::MODEL_KEY::SCORE, QString::number(app->score)},
                                    {FMH::MODEL_KEY::SMALL_PIC, small_pic},
                                    {FMH::MODEL_KEY::SUMMARY, app->summary},
                                    {FMH::MODEL_KEY::TAGS, app->tags},
                                    {FMH::MODEL_KEY::TOTAL_DOWNLOADS, QString::number(app->totalDownloads)},
                                    {FMH::MODEL_KEY::TYPE_ID, app->typeId},
                                    {FMH::MODEL_KEY::TYPE_NAME, app->typeName},
                                    {FMH::MODEL_KEY::TYPE, ""},
                                    {FMH::MODEL_KEY::VERSION, app->version},
                                    {FMH::MODEL_KEY::WAY, ""},
                                    {FMH::MODEL_KEY::XDG_TYPE, app->xdgType}});
            emit this->postItemAppended();
        }

    });
}

FMH::MODEL_LIST StoreModel::items() const
{
    return m_list;
}

QVariantMap StoreModel::getCategory() const
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

void StoreModel::setCategory(QVariantMap category)
{
    if (m_category == category)
        return;

    m_category = category;
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

QVariantMap StoreModel::getApp(const QString &id)
{
    const auto app = this->m_appMap[id];
    const auto preview_pic = app->previewPics.isEmpty() ? "" : app->previewPics.first()->pic;
    const auto small_pic = app->previewPics.isEmpty() ? "" : app->previewPics.first()->smallPic;

    QStringList d_size;
    QStringList d_link;
    QStringList d_price;
    QStringList d_arch;
    QStringList d_packageName;

    for(const auto &d : app->downloads)
    {
        d_size.append(d->size);
        d_link.append(d->link);
        d_price.append(d->price);
        d_arch.append(d->packageArch);
        d_packageName.append(d->packageName);
    }

    return FMH::toMap(FMH::MODEL{
                          {FMH::MODEL_KEY::CHANGED, app->changed},
                          {FMH::MODEL_KEY::COMMENTS, app->comments},
                          {FMH::MODEL_KEY::CREATED, app->created},
                          {FMH::MODEL_KEY::DESCRIPTION, app->description},
                          {FMH::MODEL_KEY::DETAIL_PAGE, app->detailPage},
                          {FMH::MODEL_KEY::DETAILS, app->details},
                          {FMH::MODEL_KEY::GHNS_EXCLUDED, app->ghnsExcluded},
                          {FMH::MODEL_KEY::GPG_FINGERPRINT, ""},
                          {FMH::MODEL_KEY::GPG_SIGNATURE, ""},
                          {FMH::MODEL_KEY::ID, app->id},
                          {FMH::MODEL_KEY::LANGUAGE, app->language},
                          {FMH::MODEL_KEY::LINK, d_link.join(",")},
                          {FMH::MODEL_KEY::NAME, app->name},
                          {FMH::MODEL_KEY::PACKAGE_NAME, d_packageName.join(",")},
                          {FMH::MODEL_KEY::PACKAGE_TYPE, ""},
                          {FMH::MODEL_KEY::PERSON_ID, app->personId},
                          {FMH::MODEL_KEY::PIC, ""},
                          {FMH::MODEL_KEY::PREVIEW, preview_pic},
                          {FMH::MODEL_KEY::PRICE, d_price.join(",")},
                          {FMH::MODEL_KEY::REPOSITORY, ""},
                          {FMH::MODEL_KEY::SCORE, QString::number(app->score)},
                          {FMH::MODEL_KEY::SIZE, d_size.join(",")},
                          {FMH::MODEL_KEY::SMALL_PIC, small_pic},
                          {FMH::MODEL_KEY::SUMMARY, app->summary},
                          {FMH::MODEL_KEY::TAGS, app->tags},
                          {FMH::MODEL_KEY::TOTAL_DOWNLOADS, QString::number(app->totalDownloads)},
                          {FMH::MODEL_KEY::TYPE_ID, app->typeId},
                          {FMH::MODEL_KEY::TYPE_NAME, app->typeName},
                          {FMH::MODEL_KEY::TYPE, ""},
                          {FMH::MODEL_KEY::VERSION, app->version},
                          {FMH::MODEL_KEY::WAY, ""},
                          {FMH::MODEL_KEY::XDG_TYPE, app->xdgType},
                          {FMH::MODEL_KEY::PACKAGE_ARCH, d_arch.join(",")}});
}

void StoreModel::setPage(int page)
{
    if (m_page == page)
        return;

    m_page = page;
    this->requestApps();
    emit pageChanged(m_page);
}


