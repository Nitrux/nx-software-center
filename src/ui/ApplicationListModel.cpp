#include <QDebug>
#include <QLocale>
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
    roles.insert(Icon, "app_icon");
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
    auto locale = QLocale::system();
    auto lcName = locale.bcp47Name();

    QVariant ret;
    switch (role) {
    case Id:
        ret = a.value("id");
        break;
    case Version:
        ret = a.value("version", "latest");
        break;
    case CodeName:
        ret = getLocalizedValue(lcName, a.value("name").toMap());
        break;
    case Name:
        ret = getLocalizedValue(lcName, a.value("name").toMap());
        break;
    case Description:
        ret = getLocalizedValue(lcName, a.value("description").toMap());
        break;
    case Icon:
        ret = a.value("icon");
        break;
    default:
        break;
    }

    return ret;
}
QVariant ApplicationListModel::getLocalizedValue(const QString& lcName, const QMap<QString, QVariant>& lcField) const
{
    QVariant value;
    for (QString k: lcField.keys()) {
            if (k.startsWith(lcName)) {
                value = lcField[k];
                break;
            }
        }
    if (value.isNull() && lcField.contains("null"))
            value = lcField["null"];

    if (value.isNull() && !lcField.isEmpty())
            value = lcField.values().first();
    return value;
}

void ApplicationListModel::setApplications(const QList<QVariantMap> applications) {
    beginResetModel();
    this->applications = applications;
    endResetModel();
}
