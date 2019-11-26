#include "appsmodel.h"
#include "src/stores/appimagehubstore.h"

#include <QDebug>

AppsModel::AppsModel(QObject *parent) : MauiList(parent) {
  this->appImageHubStore = new AppImageHubStore();
  this->setList();
}

FMH::MODEL_LIST AppsModel::items() const { return this->m_list; }

QString AppsModel::getCategoryUri() const { return m_categoryUri; }

void AppsModel::setCategoryUri(QString categoryUri) {
  if (m_categoryUri == categoryUri)
    return;

  m_categoryUri = categoryUri;
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

  appImageHubStore->getApplications(QList<QString>());
  connect(appImageHubStore, &Store::applicationsResponseReady,
          [=](ApplicationResponseDTO *response) {
            for (ApplicationResponseDTO::Application app :
                 response->applications) {
              this->m_list.append(QHash<FMH::MODEL_KEY, QString>{
                  {FMH::MODEL_KEY::CHANGED, ""},
                  {FMH::MODEL_KEY::COMMENTS, ""},
                  {FMH::MODEL_KEY::CREATED, ""},
                  {FMH::MODEL_KEY::DESCRIPTION, ""},
                  {FMH::MODEL_KEY::DETAIL_PAGE, ""},
                  {FMH::MODEL_KEY::DETAILS, ""},
                  {FMH::MODEL_KEY::GHNS_EXCLUDED, ""},
                  {FMH::MODEL_KEY::GPG_FINGERPRINT, ""},
                  {FMH::MODEL_KEY::GPG_SIGNATURE, ""},
                  {FMH::MODEL_KEY::ID, ""},
                  {FMH::MODEL_KEY::LANGUAGE, ""},
                  {FMH::MODEL_KEY::LINK, ""},
                  {FMH::MODEL_KEY::NAME, ""},
                  {FMH::MODEL_KEY::PACKAGE_NAME, ""},
                  {FMH::MODEL_KEY::PACKAGE_TYPE, ""},
                  {FMH::MODEL_KEY::PERSON_ID, ""},
                  {FMH::MODEL_KEY::PIC, ""},
                  {FMH::MODEL_KEY::PREVIEW, ""},
                  {FMH::MODEL_KEY::PRICE, ""},
                  {FMH::MODEL_KEY::REPOSITORY, ""},
                  {FMH::MODEL_KEY::SCORE, ""},
                  {FMH::MODEL_KEY::SIZE, ""},
                  {FMH::MODEL_KEY::SMALL_PIC, ""},
                  {FMH::MODEL_KEY::SUMMARY, ""},
                  {FMH::MODEL_KEY::TAGS, ""},
                  {FMH::MODEL_KEY::TOTAL_DOWNLOADS, ""},
                  {FMH::MODEL_KEY::TYPE_ID, ""},
                  {FMH::MODEL_KEY::TYPE_NAME, ""},
                  {FMH::MODEL_KEY::TYPE, ""},
                  {FMH::MODEL_KEY::VERSION, ""},
                  {FMH::MODEL_KEY::WAY, ""},
                  {FMH::MODEL_KEY::XDG_TYPE, ""},
                  {FMH::MODEL_KEY::PACKAGE_ARCH, ""}});
            }
          });
}
