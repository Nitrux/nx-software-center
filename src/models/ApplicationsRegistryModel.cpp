#include "ApplicationsRegistryModel.h"

#include <QDebug>

ApplicationsRegistryModel::ApplicationsRegistryModel(ApplicationsRegistry *registry, QObject *parent)
    : QAbstractListModel(parent)
    , _registry(registry)
{
    _applications = registry->getApplications();

    connect(_registry, &ApplicationsRegistry::applicationAdded, this, &ApplicationsRegistryModel::handleApplicationAdded, Qt::QueuedConnection);
    connect(_registry, &ApplicationsRegistry::applicationUpdated, this, &ApplicationsRegistryModel::handleApplicationUpdated, Qt::QueuedConnection);
    connect(_registry, &ApplicationsRegistry::applicationRemoved, this, &ApplicationsRegistryModel::handleApplicationRemoved, Qt::QueuedConnection);

    initRoles();
}

QHash<int, QByteArray> ApplicationsRegistryModel::roleNames() const
{
    return _roles;
}

void ApplicationsRegistryModel::initRoles()
{
    _roles[Id] = "id";
    _roles[Name] = "name";
    _roles[Version] = "version";
    _roles[Icon] = "icon";
    _roles[Description] = "description";
    _roles[Snapshots] = "snapshots";
    _roles[XdgCategories] = "xdg_categories";
    _roles[Bundles] = "bundles";
    _roles[LatestBundlePath] = "latest_bundle_path";
    _roles[LatestBundleSize] = "latest_bundle_size";
    _roles[UpdateAvailable] = "update_available";
    _roles[RelatedTask] = "related_task";
    _roles[Data] = "data";
}

int ApplicationsRegistryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        auto app = _applications[parent.row()];
        return app.getBundles().length();
    }

    return _applications.length();
}
QVariant ApplicationsRegistryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    if (index.row() >= 0 && index.row() < _applications.length()) {
        const auto &app = _applications.at(index.row());
        const auto &appBundles = app.getBundles();
        switch (role) {
        case Id:
            return app.getId();
        case Name:
            return app.getName();
        case Version:
            return app.getVersion();
        case Icon:
            return app.getIcon();
        case Description:
            return app.getDescription();
        case XdgCategories:
            return app.getXdgCategories();
        case UpdateAvailable:
            return resolveUpdateAvailableValue(app);
        case Bundles:
            return listBundlesFileNames(app);
        case RelatedTask:
            return resolveRelatedTask(app);
        case Data:
            return QVariant::fromValue(app);
        case LatestBundlePath:
            if (!appBundles.empty())
                return app.getBundles().first().path;
            else
                return {};
        case LatestBundleSize:
            if (!appBundles.empty())
                return appBundles.first().size;
            else
                return {};
        default:
            return {};
        }
    }
    return {};
}
bool ApplicationsRegistryModel::resolveRelatedTask(const ApplicationData &app) const
{
    return _applicationsRelatedTasks.contains(app.getId()) && _applicationsRelatedTasks.value(app.getId()).status == TaskData::RUNNING;
}
bool ApplicationsRegistryModel::resolveUpdateAvailableValue(const ApplicationData &app) const
{
    return _appliactionsUpdateData.contains(app.getId()) && _appliactionsUpdateData.value(app.getId()).updateAvailable;
}

void ApplicationsRegistryModel::handleApplicationAdded(const ApplicationData &application)
{
    int length = _applications.length();
    beginInsertRows(QModelIndex(), length, length);
    _applications.push_back(application);
    endInsertRows();
}

void ApplicationsRegistryModel::handleApplicationUpdated(const ApplicationData &application)
{
    auto row = _applications.indexOf(application);
    _applications[row] = application;

    if (row >= 0 && row < _applications.length())
        emit(dataChanged(index(row, 0), index(row + 1, 0)));
}

void ApplicationsRegistryModel::handleApplicationRemoved(const ApplicationData &application)
{
    auto idx = _applications.indexOf(application);

    beginRemoveRows(QModelIndex(), idx, idx);
    _applications.removeAt(idx);
    endRemoveRows();
}

void ApplicationsRegistryModel::handleUpdateInformation(const ApplicationUpdateData &updateData)
{
    qDebug() << "Update information received" << updateData.application.getId();
    _appliactionsUpdateData.insert(updateData.application.getId(), updateData);

    auto row = _applications.indexOf(updateData.application);
    if (row >= 0 && row < _applications.length()) {
        qDebug() << "Applications Registry Model row changed: " << row;
        emit(dataChanged(index(row, 0), index(row, 0)));
    }
}
void ApplicationsRegistryModel::handleTaskUpdate(const TaskData &notification)
{
    const auto &appId = notification.related_app_id;
    if (!appId.isEmpty()) {
        _applicationsRelatedTasks.insert(appId, notification);
        int idx = findApplicationIndexById(appId);
        if (idx >= 0 && idx < _applications.length())
            emit(dataChanged(index(idx, 0), index(idx, 0)));
    }
}
int ApplicationsRegistryModel::findApplicationIndexById(const QString &applicationId)
{
    for (int idx = 0; idx < _applications.length(); idx++) {
        const auto &application = _applications[idx];
        if (application.getId() == applicationId)
            return idx;
    }

    return -1;
}
QStringList ApplicationsRegistryModel::listBundlesFileNames(const ApplicationData &data) const
{
    QStringList result;
    for (const auto &bundle : data.getBundles()) {
        QUrl bundleUrl = QUrl::fromLocalFile(bundle.path);
        QString fileName = bundleUrl.fileName();
        fileName = fileName.remove(".AppImage", Qt::CaseInsensitive);
        result.append(fileName);
    }

    return result;
}
