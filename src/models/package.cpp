#include "package.h"
#include "utils/appimagetools.h"
#include "ResponseDTO/application.h"
#include "nx.h"

//#include <QFile>

Package::Package(App *appSource, QObject *parent) : QObject(parent)
  ,m_appSource(appSource)
  ,m_downloader (new FMH::Downloader(this))
{
    connect(m_downloader, &FMH::Downloader::progress, this, &Package::setProgress);
    connect(m_downloader, &FMH::Downloader::fileSaved, [this](QString path)
    {
        this->integratePackage(path);
        emit this->isRunningChanged();
        emit this->isFinishedChanged();
      emit this->progressFinished();

    });

    connect(m_downloader, &FMH::Downloader::warning, [this](QString warning)
    {
        emit this->progressError(warning);
        emit this->isRunningChanged();
        emit this->isFinishedChanged();
    });
}

void Package::stop()
{
    if(this->isRunning())
    {
        m_downloader->stop();
        emit this->progressError("Package download has been aborted.");
        setProgress(0);
        emit this->isRunningChanged();
        emit this->isFinishedChanged();
    }
}

void Package::setPackageIndex(const int &index)
{
    if(index > m_appSource->getData()->downloads.size() || index < 0)
        return;

    this->m_packageIndex = index;
    this->m_link = m_appSource->getData()->downloads.at(this->m_packageIndex)->link;
    this->m_package = m_appSource->m_downloads.at(this->m_packageIndex).toMap();

    emit this->linkChanged(this->m_link);
    emit this->packageChanged(this->m_package);
    emit this->packagedIndexChanged(this->m_packageIndex);
}

void Package::setProgress(const int &progress)
{
    this->m_progress = progress;
    emit this->progressChanged(this->m_progress);
}

int Package::getPackageIndex() const
{
    return m_packageIndex;
}

QUrl Package::getLink() const
{
    return m_link;
}

QVariantMap Package::getPackage() const
{
    return m_package;
}

QUrl Package::getPath() const
{
    return m_path;
}

App *Package::appSource() const
{
    return m_appSource;
}

void Package::integratePackage(const QString &path)
{
    this->setPath(path); //adds local file scheme to the path file:// path ->m_path

    if(!FMH::fileExists(m_path))
        return;
    qDebug() << "integrate this appimage" << path << m_path;

//    QFile::setPermissions (m_path.toLocalFile (), {QFileDevice::ExeUser | QFileDevice::ExeOwner | QFileDevice::ExeGroup});

    AppImageTools::integrate(m_path);
}

void Package::updatePackage()
{

}

void Package::removePackage()
{

}

void Package::installPackage()
{
    const auto package =   m_appSource->getData()->downloads.at(this->m_packageIndex);

    qDebug() << "TRY TO DOWNLOAD LINK" << this->m_link;

    auto appimagePath = NX::AppsPath.toString()+("/")+package->name;

    m_downloader->downloadFile(this->m_link, appimagePath);
}

void Package::launchPackage()
{
    qDebug() << "launch the package if it is locally avaliable" << m_path;
    if(FMH::fileExists(m_path))
    {
        qDebug() << "launch the package if it is locally avaliable EXISTS" << m_path;

        FMStatic::openUrl(m_path);
    }
}

void Package::buyPackage()
{

}

bool Package::isRunning() const
{
    return m_downloader->isRunning();
}

bool Package::isFinished() const
{
    return m_downloader->isFinished();
}

void Package::setPath(const QString &path)
{
    m_path = QUrl::fromUserInput(path);
    emit pathChanged(m_path);

}
