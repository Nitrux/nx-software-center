#include "progressmanager.h"
#include "ResponseDTO/application.h"

bool ProgressManager::contains(const App &app, const int &packageIndex) const
{
    if(packageIndex > app.getData()->downloads.size() || packageIndex < 0)
        return false;

    return this->m_packages.contains(app.getData()->downloads.at(packageIndex)->link);
}

QHash<int, QByteArray> ProgressManager::roleNames() const
{
    QHash<int, QByteArray> roles;
     roles[Qt::UserRole + 1] = "item";
     return roles;
}

ProgressManager::ProgressManager(QObject *parent) : QAbstractListModel(parent),
    m_count(0)
{
    emit this->countChanged(this->m_count);
}

int ProgressManager::rowCount(const QModelIndex &) const
{
    return this->m_list.size();
}

QVariant ProgressManager::data(const QModelIndex &index, int role) const
{
    auto item = m_list.at(index.row());
    qDebug()<< "REQUESTING PACKAGE FOR MDOEL"<< index << item->getData()->name;
    return QVariant::fromValue(item);
}

Package *ProgressManager::appendPackage(App *app, const int &packageIndex)
{
    if(this->contains(*app, packageIndex))
    {
        emit this->warning(tr("Package is already in the Progress List"));
        return new Package;
    }
    const auto package = new Package(*app, this);
    package->setPackageIndex(packageIndex);
    qDebug()<< "APPNDIGN PACKAGE" << package->getData()->downloads.at(packageIndex)->name;

    beginInsertRows(QModelIndex(), this->m_list.size(), this->m_list.size());
    this->m_packages[package->getData()->downloads.at(packageIndex)->link] = package;
    this->m_list << package;


    this->m_count = this->m_list.size();
    emit this->countChanged(this->m_count);
    endInsertRows();

    return package;
}

void ProgressManager::removePackage(App *app, const int &packageIndex)
{
    if(contains(*app, packageIndex))
    {
        auto package =  this->m_packages[app->getData()->downloads.at(packageIndex)->link];
        package->stop();
        package->deleteLater();

        this->m_packages.remove(app->getData()->downloads.at(packageIndex)->link);
        this->m_count = this->m_packages.size();
        emit this->countChanged(this->m_count);
    }
}

Package *ProgressManager::takePackage(App *app, const int &packageIndex)
{
    if(contains(*app, packageIndex))
    {
        auto package =  this->m_packages[app->getData()->downloads.at(packageIndex)->link];
        package->stop();

        this->m_packages.remove(app->getData()->downloads.at(packageIndex)->link);
        this->m_count = this->m_packages.size();
        emit this->countChanged(this->m_count);

        return std::move(package);
    }

    return new Package;
}

void Package::stop()
{

}

void Package::setPackageIndex(const int &index)
{
    if(index > this->getData()->downloads.size() || index < 0)
        return;

    this->m_packageIndex = index;
    this->m_link = this->getData()->downloads.at(this->m_packageIndex)->link;
    this->m_package = this->m_downloads.at(this->m_packageIndex).toMap();

    emit this->linkChanged(this->m_link);
    this->packageChanged(this->m_package);
    emit this->packagedIndexChanged(this->m_packageIndex);
}

int Package::getPackageIndex() const
{
    return m_packageIndex;
}

QString Package::getLink() const
{
    return m_link;
}

QVariantMap Package::getPackage() const
{
    return m_package;
}
