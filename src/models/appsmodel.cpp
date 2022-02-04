#include "appsmodel.h"

#include <QDebug>
#include <QTimer>

#include <MauiKit/FileBrowsing/fmstatic.h>
#include <MauiKit/FileBrowsing/fileloader.h>

#include "utils/appimagetools.h"

AppsModel::AppsModel(QObject *parent) : MauiList(parent)
{
    auto timer = new  QTimer (this);

    timer->setSingleShot(true);
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, this, &AppsModel::setList);
}

void AppsModel::componentComplete()
{
    setList();
}

void AppsModel::setAppUpdatable(int index) {
    emit this->preListChanged();

    this->m_list[index].insert(FMH::MODEL_KEY::UPDATABLE, "true");

    emit this->postListChanged();
}

FMH::MODEL_LIST AppsModel::getItems() {
    return this->m_list;
}

const FMH::MODEL_LIST &AppsModel::items() const { return this->m_list; }

void AppsModel::launchApp(const int &index)
{

    const auto url = this->get(index).value("url").toUrl();
    qDebug() << "try to launch appimage" << url;
    FMStatic::openUrl(url);
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

void AppsModel::setIsUpdateAvailable(bool isUpdateAvailable) {
    if ( this->m_isUpdateAvailable != isUpdateAvailable ) {
        this->m_isUpdateAvailable = isUpdateAvailable;

        emit isUpdateAvailableChanged();
    }
}