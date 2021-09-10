#include "progressmanager.h"

#include "ResponseDTO/application.h"

#include <MauiKit/FileBrowsing/fmstatic.h>


bool ProgressManager::contains(const App &app, const int &packageIndex) const
{
    if(packageIndex > app.getData()->downloads.size() || packageIndex < 0)
        return false;

    return this->m_packages.contains(app.getData()->downloads.at(packageIndex)->link);
}

QHash<int, QByteArray> ProgressManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ROLES::ITEM] = "item";
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

    switch (role)
    {
    case ROLES::ITEM:
        return QVariant::fromValue(item);
    default:
        return QVariant();
    }
}

Package *ProgressManager::appendPackage(App *app, const int &packageIndex)
{
    if(this->contains(*app, packageIndex))
    {
        emit this->warning(tr("Package is already in the Progress List"));
        return nullptr;
    }

    const auto package = new Package(app, this);
    package->setPackageIndex(packageIndex);

    beginInsertRows(QModelIndex(), this->m_list.size(), this->m_list.size());

    this->m_packages[package->getLink().toString()] = package;
    this->m_list << package;
    package->installPackage();

    endInsertRows();

    this->m_count = this->m_list.size();
    emit this->countChanged(this->m_count);

    return package;
}

void ProgressManager::removePackage(const int &packageIndex)
{
    if(packageIndex >= this->m_list.size() || packageIndex < 0)
    {
        return;
    }

    auto link = this->m_list.at(packageIndex)->getLink().toString();
    auto package =  this->m_packages[link];

    if(package->isRunning())
    {
        return;
    }

    beginRemoveRows(QModelIndex(), packageIndex, packageIndex);

    this->m_packages.remove(link);
    this->m_list.removeAt(packageIndex);

    package->deleteLater();

    this->m_count = this->m_packages.size();
    emit this->countChanged(this->m_count);
    endRemoveRows();
}

void ProgressManager::stopPackage(const int &packageIndex)
{
    if(packageIndex >= this->m_list.size() || packageIndex < 0)
    {
        return;
    }
    auto link = this->m_list.at(packageIndex)->getLink().toString();
    qDebug()<< link;
    this->m_packages[link]->stop();
}

Package *ProgressManager::takePackage(const int &packageIndex)
{
    if(packageIndex >= this->m_list.size() || packageIndex < 0)
    {
        return nullptr;
    }
    auto link = this->m_list.at(packageIndex)->getLink().toString();

    auto package =  this->m_packages[link];
    package->stop();

    this->m_packages.remove(link);
    this->m_count = this->m_packages.size();
    emit this->countChanged(this->m_count);

    return std::move(package);
}

