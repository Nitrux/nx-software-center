#include "ApplicationListModel.h"

ApplicationListModel::ApplicationListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> ApplicationListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Id, "app_id");
    roles.insert(CodeName, "app_code_name");
    roles.insert(Name, "app_name");
    roles.insert(Description, "app_description");
    roles.insert(Version, "app_version");
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

    Application a = applications.at(index.row());
    // FIXME: Implement more
    QVariant ret;
    switch (role) {
    case Id:
        ret = a.getId();
        break;
    case Version:
        ret = a.getVersion();
        break;
    case CodeName:
        ret = a.getCodeName();
        break;
    case Name:
        ret = a.getName();
        break;
    case Description:
        ret = a.getDescription();
        break;
    default:
        break;
    }

    return ret;
}

void ApplicationListModel::setApplications(const QList<Application> &applications) {
    beginResetModel();
    this->applications = applications;
    endResetModel();
}
