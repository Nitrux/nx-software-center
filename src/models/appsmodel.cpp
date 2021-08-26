#include "appsmodel.h"

#include <QDebug>
#include <QProcess>

#include <MauiKit/FileBrowsing/fmstatic.h>
#include <MauiKit/FileBrowsing/fileloader.h>

#include <QFileSystemWatcher>

AppsModel::AppsModel(QObject *parent) : MauiList(parent)
  , m_watcher(new QFileSystemWatcher(this))
{
    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &AppsModel::setList);
    m_watcher->addPath(QUrl(FMStatic::HomePath+"/Applications").toLocalFile());
}

void AppsModel::componentComplete()
{
   setList();
}

const FMH::MODEL_LIST &AppsModel::items() const { return this->m_list; }

void AppsModel::launchApp(const int &index) {

    const auto url = this->get(index).value("url").toUrl();
qDebug() << "try to launch appimage" << url;
    FMStatic::openUrl(url);

//    {
//        emit appLaunchSuccess();

//    }else
//    {
//        emit appLaunchError(0);
//    }
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
    fileLoader->informer = &FMStatic::getFileInfoModel;

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

    QProcess *appProcess = new QProcess(this);
    appProcess->start("ail-cli", {"unintegrate", url.toLocalFile()});

    connect(appProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError err) {
        qDebug() << "UNINTEGRATE QPROCESS ERROR" << err;

    });
    connect(appProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << "UNINTEGRATE QPROCESS FINISHED" << exitCode << exitStatus;
    });
}

void AppsModel::clear()
{
    emit this->preListChanged();
    this->m_list.clear();
    emit this->postListChanged();
    emit this->countChanged();
}

