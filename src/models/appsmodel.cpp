#include "appsmodel.h"

AppsModel::AppsModel(QObject * parent) : MauiList(parent)
{
//  this->setList();
}

FMH::MODEL_LIST AppsModel::items() const
{
  return this->m_list;
}

QString AppsModel::getCategoryUri() const
{
  return m_categoryUri;
}

void AppsModel::setCategoryUri(QString categoryUri)
{
  if (m_categoryUri == categoryUri)
    return;

  m_categoryUri = categoryUri;
  emit categoryUriChanged(m_categoryUri);
}

void AppsModel::setList()
{
  this->m_list = {
    /*1*/
    {
      {FMH::MODEL_KEY::NAME, "Index"},
      {FMH::MODEL_KEY::CATEGORY, "Installed"},
      {FMH::MODEL_KEY::VERSION, "1.0.1"},
      //                   {FMH::MODEL_KEY::AUTHOR, "Camilo Higuita"},
      {FMH::MODEL_KEY::ORG, "Mauikit"},
      {FMH::MODEL_KEY::IMG, "qrc:/tests/banner_index.png"},
      {FMH::MODEL_KEY::ICON, "index"},
      {FMH::MODEL_KEY::COUNT, "123"},
      {FMH::MODEL_KEY::MODIFIED, "1.0.1"},
      {FMH::MODEL_KEY::LICENSE, "GPL"},
      {FMH::MODEL_KEY::DESCRIPTION, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper. Curabitur fringilla rutrum mauris, in imperdiet dui commodo vitae. Fusce at enim ullamcorper, tincidunt orci a, tincidunt urna. Vivamus quis est ut elit dignissim semper. Vivamus erat leo, iaculis id faucibus ac, placerat eu lacus. Praesent arcu tortor, aliquet at volutpat eu, rutrum eget ante. Vivamus sed lorem sed massa interdum convallis. Suspendisse lacinia augue odio, id auctor ipsum varius id. Nam sit amet rhoncus augue. Vestibulum at lobortis velit. Proin ipsum eros, vehicula ut tempus vel, aliquam a orci. Fusce facilisis pulvinar orci. "},
      {FMH::MODEL_KEY::LICENSE, "GPL"}
    },
    /*2*/
    {  {FMH::MODEL_KEY::NAME, "Index"},
       {FMH::MODEL_KEY::CATEGORY, "Installed"},
       {FMH::MODEL_KEY::VERSION, "1.0.1"},
       //                   {FMH::MODEL_KEY::AUTHOR, "Camilo Higuita"},
       {FMH::MODEL_KEY::ORG, "Mauikit"},
       {FMH::MODEL_KEY::IMG, "qrc:/tests/banner_index.png"},
       {FMH::MODEL_KEY::ICON, "index"},
       {FMH::MODEL_KEY::COUNT, "123"},
       {FMH::MODEL_KEY::MODIFIED, "1.0.1"},
       {FMH::MODEL_KEY::LICENSE, "GPL"},
       {FMH::MODEL_KEY::DESCRIPTION, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper. Curabitur fringilla rutrum mauris, in imperdiet dui commodo vitae. Fusce at enim ullamcorper, tincidunt orci a, tincidunt urna. Vivamus quis est ut elit dignissim semper. Vivamus erat leo, iaculis id faucibus ac, placerat eu lacus. Praesent arcu tortor, aliquet at volutpat eu, rutrum eget ante. Vivamus sed lorem sed massa interdum convallis. Suspendisse lacinia augue odio, id auctor ipsum varius id. Nam sit amet rhoncus augue. Vestibulum at lobortis velit. Proin ipsum eros, vehicula ut tempus vel, aliquam a orci. Fusce facilisis pulvinar orci. "},
       {FMH::MODEL_KEY::LICENSE, "GPL"}
    }};
}
