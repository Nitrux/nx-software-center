#include "appsmodel.h"

#include <QDebug>
#include <QTimer>

#include <MauiKit/FileBrowsing/fmstatic.h>
#include <MauiKit/FileBrowsing/fileloader.h>

#include <QFileSystemWatcher>

#include "utils/appimagetools.h"

AppsModel::AppsModel(QObject *parent) : MauiList(parent)
  , m_watcher(new QFileSystemWatcher(this))
{
    auto timer = new  QTimer (this);

    timer->setSingleShot(true);
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, this, &AppsModel::setList);
    connect(m_watcher, &QFileSystemWatcher::directoryChanged, [timer](QString )
    {
        timer->start();
    });

    m_watcher->addPath(QUrl(FMStatic::HomePath+"/Applications").toLocalFile());
}

void AppsModel::componentComplete()
{
    setList();
}

const FMH::MODEL_LIST &AppsModel::items() const { return this->m_list; }

void AppsModel::launchApp(const int &index)
{

    const auto url = this->get(index).value("url").toUrl();
    qDebug() << "try to launch appimage" << url;
    FMStatic::openUrl(url);
}

void AppsModel::updateApp(const int &index)
{

    const auto url = this->get(index).value("url").toUrl();
    qDebug() << "try to update appimage" << url;

    QString appImagePath = QString(url.toLocalFile());
    qDebug() << appImagePath;

    updater = new QAppImageUpdate(appImagePath, /*singleThreaded=*/false, /*parent=*/this);
    
    connect(updater, &QAppImageUpdate::error, this, &AppsModel::handleError);
    connect(updater, &QAppImageUpdate::finished, this, &AppsModel::handleFinished);

    updater->setShowLog(false);
    
    updater->start(QAppImageUpdate::Action::CheckForUpdate); /* Check for update. */
}

void AppsModel::removeApp(const int &index) {

    const auto url = this->get(index).value("url").toUrl();

    //    emit preItemRemoved(index);

    if(FMStatic::removeFiles({url}))
    {
        unintegrate(url);
        this->m_list.removeAt(index);
        emit appDeleteSuccess();
    }

    //    emit postItemRemoved();
}

void AppsModel::resfresh()
{
    this->setList();
}

void AppsModel::setList()
{
    this->clear();

    FMH::FileLoader *fileLoader = new FMH::FileLoader;
    fileLoader->informer = &AppImageTools::desktopData;

    connect(fileLoader, &FMH::FileLoader::finished, [=](FMH::MODEL_LIST items, QList<QUrl>) {

        emit this->preListChanged();
        this->m_list << items;
        emit this->postListChanged();
        emit this->countChanged();

    });

    fileLoader->requestPath({FMStatic::HomePath+"/Applications"}, false, {"*.appimage"});
}

void AppsModel::unintegrate(const QUrl &url)
{
    if(!FMH::fileExists(url))
        return;
    qDebug() << "unintegrate this appimage" << url;

    AppImageTools::unintegrate(url);
}

void AppsModel::clear()
{
    emit this->preListChanged();
    this->m_list.clear();
    emit this->postListChanged();
    emit this->countChanged();
}

void AppsModel::handleError(short errorCode, short action)
{
    if(action == QAppImageUpdate::Action::Update) {
        qInfo() << "AppsModel::handleError # " << QAppImageUpdate::errorCodeToString(errorCode);
        emit appUpdateError("AppImage update error.\n\nError Message:"+QAppImageUpdate::errorCodeToString(errorCode));
    } else if ( action == QAppImageUpdate::Action::CheckForUpdate ) {
        qInfo() << "AppsModel::handleError # " << QAppImageUpdate::errorCodeToString(errorCode);
        emit appUpdateError("AppImage check for update error.\n\nError Message:"+QAppImageUpdate::errorCodeToString(errorCode));
    }

    return;
}

void AppsModel::handleFinished(QJsonObject info, short action)
{
    if(action == QAppImageUpdate::Action::Update) {
        qInfo() << "AppsModel::handleFinished # Update:: " << info;
        emit appUpdateSuccess("AppImage updated successfully.");
    } else if(action == QAppImageUpdate::Action::CheckForUpdate) {
        qInfo() << "AppsModel::handleFinished # CheckForUpdate:: " << info;

        if ( info.value("UpdateAvailable") == true ) {
            updater->start(QAppImageUpdate::Action::Update); /* Start the update. */
        } else {
            qInfo() << "AppsModel::handleFinished # AppImage is already updated and latest";
            emit appUpdateError("AppImage is already updated and latest.");
        }
    }

    return;
}