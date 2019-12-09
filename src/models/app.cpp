#include "app.h"
#include "ResponseDTO/application.h"

App::App(QObject *parent) : QObject(parent), m_data(new Application(this)), downloader(nullptr)
{
}

App::App(const App &other, QObject *parent) : QObject(parent), m_data(other.m_data),
    m_info(other.m_info), m_downloads(other.m_downloads), m_images(other.m_images), m_urls(other.m_urls), m_id(other.m_id), m_isInstalled(other.m_isInstalled), m_isUpdatable(other.m_isUpdatable)
{
    qDebug()<< "COPYING APP";
}

Application *App::getData() const
{
    return m_data;
}

void App::setModels()
{
    if(this->m_data == nullptr)
        return;

    //the models
    for(const auto &data : this->m_data->previewPics)
        this->m_images << QVariantMap {
    {FMH::MODEL_NAME[FMH::MODEL_KEY::PIC], data->pic},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::SMALL_PIC], data->pic}};
emit this->imagesChanged(this->m_images);

for(const auto data : this->m_data->previewUrls)
  this->m_urls << QVariantMap {{FMH::MODEL_NAME[FMH::MODEL_KEY::PREVIEW], data->preview}};

qDebug()<< m_images;

for(const auto &data : this->m_data->downloads)
  this->m_downloads << QVariantMap {
    {FMH::MODEL_NAME[FMH::MODEL_KEY::PACKAGE_ARCH], data->packageArch},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::PACKAGE_TYPE], data->packageType},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::GPG_SIGNATURE], data->gpgSignature},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::GPG_FINGERPRINT], data->gpgFingerprint},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::LINK], data->link},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::NAME], data->name},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::PACKAGE_NAME], data->packageName},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::PRICE], data->price},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::REPOSITORY], data->repository},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::PRICE], data->price},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::SIZE], data->size},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::TYPE], data->type},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::WAY], data->way},
    {FMH::MODEL_NAME[FMH::MODEL_KEY::TAGS], data->tags}};
emit this->downloadsChanged(this->m_downloads);

this->m_info = FMH::toMap(FMH::MODEL{
                              {FMH::MODEL_KEY::CHANGED, m_data->changed},
                              {FMH::MODEL_KEY::COMMENTS, m_data->comments},
                              {FMH::MODEL_KEY::CREATED, m_data->created},
                              {FMH::MODEL_KEY::DESCRIPTION, m_data->description},
                              {FMH::MODEL_KEY::DETAIL_PAGE, m_data->detailPage},
                              {FMH::MODEL_KEY::DETAILS, m_data->details},
                              {FMH::MODEL_KEY::GHNS_EXCLUDED, m_data->ghnsExcluded},
                              {FMH::MODEL_KEY::ID, m_data->id},
                              {FMH::MODEL_KEY::LANGUAGE, m_data->language},
                              {FMH::MODEL_KEY::NAME, m_data->name},
                              {FMH::MODEL_KEY::PERSON_ID, m_data->personId},
                              {FMH::MODEL_KEY::SCORE, QString::number(m_data->score)},
                              {FMH::MODEL_KEY::SUMMARY, m_data->summary},
                              {FMH::MODEL_KEY::TAGS, m_data->tags},
                              {FMH::MODEL_KEY::TOTAL_DOWNLOADS, QString::number(m_data->totalDownloads)},
                              {FMH::MODEL_KEY::TYPE_ID, m_data->typeId},
                              {FMH::MODEL_KEY::TYPE_NAME, m_data->typeName},
                              {FMH::MODEL_KEY::VERSION, m_data->version},
                              {FMH::MODEL_KEY::XDG_TYPE, m_data->xdgType}});
emit this->infoChanged(this->m_info);
}

void App::updateApp()
{

}

void App::removeApp()
{

}

void App::installApp(const int &packageIndex)
{
    if(packageIndex >  this->m_data->downloads.size() || packageIndex < 0)
        return;
    const auto package =   this->m_data->downloads.at(packageIndex);
    this->downloader = package->downloadFile(FMH::DownloadsPath+("/")+package->name);
}

void App::launchApp()
{

}

void App::buyApp()
{

}

void App::setData(Application *data)
{
    if (m_data == data || !data)
        return;

    qDebug()<< "Setting app model" << data->name;
    this->m_data = data;
    this->m_id = data->id;
    this->setModels();
    emit dataChanged(m_data);
}
