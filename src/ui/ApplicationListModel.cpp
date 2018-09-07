#include <QDebug>
#include <QLocale>
#include "ApplicationListModel.h"
#include "entities/LocalizationUtils.h"

ApplicationListModel::ApplicationListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> ApplicationListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Id, "app_id");
    roles.insert(Name, "app_name");
    roles.insert(Icon, "app_icon");
    roles.insert(Abstract, "app_abstract");
    roles.insert(Version, "app_version");
    roles.insert(Size, "app_size");

    return roles;
}

int ApplicationListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return applications.size();
}

QVariant ApplicationListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || applications.size() <= index.row())
        return QVariant();

    auto a = applications.at(index.row());

    QVariant ret;
    switch (role) {
    case Id:
        ret = a.id;
        break;
    case Version:
        ret = a.latestReleaseVersion;
        break;
    case Size:
        ret = a.fileSize;
        break;
    case Name:
        ret = LocalizationUtils::getLocalizedValue(a.name);
        break;
    case Abstract:
        ret = LocalizationUtils::getLocalizedValue(a.abstract);
        break;
    case Icon:
        ret = a.icon;
        break;
    default:
        break;
    }

    return ret;
}

void ApplicationListModel::setApplications(const QList<ApplicationAbstract> applications) {
    beginResetModel();
    this->applications = applications;
    endResetModel();
}
