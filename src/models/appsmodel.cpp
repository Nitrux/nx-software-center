#include "appsmodel.h"

#include <QDebug>
#include <QRegularExpression>
#include <QPixmap>
#include <QProcess>
#include <KIO/ListJob>
#include <KIO/PreviewJob>
#include <KIO/DeleteJob>
#include <KFileItem>

#include <MauiKit/fmstatic.h>
#include <MauiKit/fileloader.h>

AppsModel::AppsModel(QObject *parent) : MauiList(parent) {}

void AppsModel::componentComplete() {    
   setList();
}

const FMH::MODEL_LIST &AppsModel::items() const { return this->m_list; }

void AppsModel::launchApp(const int &index) {

    const auto url = this->get(index).value("url").toUrl();

    if(FMStatic::openUrl(url))
    {
        emit appLaunchSuccess();

    }else
    {
        emit appLaunchError(0);
    }
}

void AppsModel::removeApp(const int &index) {

    const auto url = this->get(index).value("url").toUrl();

    emit preItemRemoved(index);

    if(FMStatic::removeFiles({url}))
    {
        emit appDeleteSuccess();
    }

    emit postItemRemoved();
}

void AppsModel::resfresh()
{
    this->setList();
}

void AppsModel::setList()
{
    emit preListChanged();
    this->m_list.clear();
    emit postListChanged();

    FMH::FileLoader *fileLoader = new FMH::FileLoader;
    fileLoader->informer = &FMH::getFileInfoModel;

    connect(fileLoader, &FMH::FileLoader::finished, [=](FMH::MODEL_LIST items, QList<QUrl>) {

        emit this->preItemsAppended(items.size());

        this->m_list << items;

        emit this->postItemAppended();

    });

    fileLoader->requestPath({FMH::HomePath+"/Applications"}, false, {"*.appimage"});
}

