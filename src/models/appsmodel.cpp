#include "appsmodel.h"
#include "src/stores/appimagehubstore.h"

#include <QDebug>

AppsModel::AppsModel(QObject *parent) : MauiList(parent),
    m_store(new AppImageHubStore(this))
{
    connect(this->m_store, &Store::applicationsResponseReady,
            [=](ApplicationResponseDTO *response) {
qDebug()<< "APPS RESP)OMNSER READY";
        emit this->preListChanged();
              for (const auto &app :response->applications)
              {
                  //preview pics
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

                this->m_list.append(QHash<FMH::MODEL_KEY, QString>{
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

              emit this->postListChanged();
            });

}

static QString QListToString(const QList<QString> &data)
{
   QStringList res;
   for(const auto &value : data)
       res.append(value);

   return res.join(",");
}

void AppsModel::componentComplete()
{
}

FMH::MODEL_LIST AppsModel::items() const { return this->m_list; }

QString AppsModel::getCategoryUri() const { return m_categoryUri; }

void AppsModel::setCategoryUri(QString categoryUri) {
  if (m_categoryUri == categoryUri)
    return;

  m_categoryUri = categoryUri;

  emit this->preListChanged();
  this->m_list.clear();
  emit this->postListChanged();

  this->m_store->getApplications({m_categoryUri});

  emit categoryUriChanged(m_categoryUri);
}

void AppsModel::setList() {
  qDebug() << "Set List called";

  //  this->m_list = {
  //    /*1*/
  //    {
  //      {FMH::MODEL_KEY::NAME, "Index"},
  //      {FMH::MODEL_KEY::CATEGORY, "Installed"},
  //      {FMH::MODEL_KEY::VERSION, "1.0.1"},
  //      //                   {FMH::MODEL_KEY::AUTHOR, "Camilo Higuita"},
  //      {FMH::MODEL_KEY::ORG, "Mauikit"},
  //      {FMH::MODEL_KEY::IMG, "qrc:/tests/banner_index.png"},
  //      {FMH::MODEL_KEY::ICON, "index"},
  //      {FMH::MODEL_KEY::COUNT, "123"},
  //      {FMH::MODEL_KEY::MODIFIED, "1.0.1"},
  //      {FMH::MODEL_KEY::LICENSE, "GPL"},
  //      {FMH::MODEL_KEY::DESCRIPTION, "Lorem ipsum dolor sit amet, consectetur
  //      adipiscing elit. Integer eleifend est lectus, quis fringilla nisl
  //      luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex
  //      ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed
  //      ullamcorper. Curabitur fringilla rutrum mauris, in imperdiet dui
  //      commodo vitae. Fusce at enim ullamcorper, tincidunt orci a, tincidunt
  //      urna. Vivamus quis est ut elit dignissim semper. Vivamus erat leo,
  //      iaculis id faucibus ac, placerat eu lacus. Praesent arcu tortor,
  //      aliquet at volutpat eu, rutrum eget ante. Vivamus sed lorem sed massa
  //      interdum convallis. Suspendisse lacinia augue odio, id auctor ipsum
  //      varius id. Nam sit amet rhoncus augue. Vestibulum at lobortis velit.
  //      Proin ipsum eros, vehicula ut tempus vel, aliquam a orci. Fusce
  //      facilisis pulvinar orci. "}, {FMH::MODEL_KEY::LICENSE, "GPL"}
  //    },
  //    /*2*/
  //    {  {FMH::MODEL_KEY::NAME, "Index"},
  //       {FMH::MODEL_KEY::CATEGORY, "Installed"},
  //       {FMH::MODEL_KEY::VERSION, "1.0.1"},
  //       //                   {FMH::MODEL_KEY::AUTHOR, "Camilo Higuita"},
  //       {FMH::MODEL_KEY::ORG, "Mauikit"},
  //       {FMH::MODEL_KEY::IMG, "qrc:/tests/banner_index.png"},
  //       {FMH::MODEL_KEY::ICON, "index"},
  //       {FMH::MODEL_KEY::COUNT, "123"},
  //       {FMH::MODEL_KEY::MODIFIED, "1.0.1"},
  //       {FMH::MODEL_KEY::LICENSE, "GPL"},
  //       {FMH::MODEL_KEY::DESCRIPTION, "Lorem ipsum dolor sit amet,
  //       consectetur adipiscing elit. Integer eleifend est lectus, quis
  //       fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis
  //       massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur
  //       volutpat id turpis sed ullamcorper. Curabitur fringilla rutrum
  //       mauris, in imperdiet dui commodo vitae. Fusce at enim ullamcorper,
  //       tincidunt orci a, tincidunt urna. Vivamus quis est ut elit dignissim
  //       semper. Vivamus erat leo, iaculis id faucibus ac, placerat eu lacus.
  //       Praesent arcu tortor, aliquet at volutpat eu, rutrum eget ante.
  //       Vivamus sed lorem sed massa interdum convallis. Suspendisse lacinia
  //       augue odio, id auctor ipsum varius id. Nam sit amet rhoncus augue.
  //       Vestibulum at lobortis velit. Proin ipsum eros, vehicula ut tempus
  //       vel, aliquam a orci. Fusce facilisis pulvinar orci. "},
  //       {FMH::MODEL_KEY::LICENSE, "GPL"}
  //    }};


}
