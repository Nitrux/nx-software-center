#include "progressmanager.h"
#include "ResponseDTO/application.h"
#include "nx.h"

#include <QProcess>

#include <MauiKit/FileBrowsing/fmstatic.h>
#include <MauiKit/FileBrowsing/downloader.h>

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
    roles[ROLES::MODE] = "mode";
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
    case ROLES::MODE:
        qDebug()<< "REQUESTING MODE <<" << item->getModelLabel();
        return item->getModelLabel();
    default:
        return QVariant();
    }
}

Package *ProgressManager::appendPackage(App *app, const int &packageIndex, const uint &mode)
{
    if(this->contains(*app, packageIndex))
    {
        emit this->warning(tr("Package is already in the Progress List"));
        return new Package;
    }
    const auto package = new Package(*app, this);
    package->setPackageIndex(packageIndex);
    package->setMode(static_cast<Package::MODE>(mode));

    beginInsertRows(QModelIndex(), this->m_list.size(), this->m_list.size());

    this->m_packages[package->getLink().toString()] = package;
    this->m_list << package;
    this->m_count = this->m_list.size();
    emit this->countChanged(this->m_count);

    switch (static_cast<Package::MODE>(mode))
    {
    case Package::MODE::DOWNLOAD:
        package->installPackage();
        break;
    case Package::MODE::UPDATE:
        package->updatePackage();
        break;
    case Package::MODE::REMOVE:
        package->removePackage();
        break;
    case Package::MODE::LAUNCH:
        package->launchPackage();
        break;
    default: return package;
    }

    endInsertRows();

    return package;
}

void ProgressManager::removePackage(App *app, const int &packageIndex)
{
    if(contains(*app, packageIndex))
    {
        auto package =  this->m_packages[app->getData()->downloads.at(packageIndex)->link];
        package->stop();
        package->deleteLater();

        this->m_packages.remove(app->getData()->downloads.at(packageIndex)->link);
        this->m_count = this->m_packages.size();
        emit this->countChanged(this->m_count);
    }
}

Package *ProgressManager::takePackage(App *app, const int &packageIndex)
{
    if(contains(*app, packageIndex))
    {
        auto package =  this->m_packages[app->getData()->downloads.at(packageIndex)->link];
        package->stop();

        this->m_packages.remove(app->getData()->downloads.at(packageIndex)->link);
        this->m_count = this->m_packages.size();
        emit this->countChanged(this->m_count);

        return std::move(package);
    }

    return new Package;
}

void Package::stop()
{

}

void Package::setPackageIndex(const int &index)
{
    if(index > this->getData()->downloads.size() || index < 0)
        return;

    this->m_packageIndex = index;
    this->m_link = this->getData()->downloads.at(this->m_packageIndex)->link;
    this->m_package = this->m_downloads.at(this->m_packageIndex).toMap();

    emit this->linkChanged(this->m_link);
    emit this->packageChanged(this->m_package);
    emit this->packagedIndexChanged(this->m_packageIndex);
}

static const QHash<Package::MODE, QString> MODE_MAP = {
    {Package::MODE::DOWNLOAD, "Download"},
    {Package::MODE::LAUNCH, "Launch"},
    {Package::MODE::REMOVE, "Remove"},
    {Package::MODE::UPDATE, "Update"}
};

void Package::setMode(const Package::MODE &mode)
{
    this->m_mode = mode;
    emit this->modeChanged(this->m_mode);

    this->m_modeLabel = MODE_MAP[this->m_mode];
    emit this->modeLabelChanged(this->m_modeLabel);
    qDebug()<< "Setting mode"<<  m_modeLabel;

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

QString Package::getModelLabel() const
{
    return this->m_modeLabel;
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

void Package::integratePackage(const QString &path)
{
    this->setPath(path);

    if(!FMH::fileExists(m_path))
        return;
    qDebug() << "integrate this appimage" << path << m_path;

    QProcess *appProcess = new QProcess(this);
    appProcess->start("ail-cli", {"integrate", m_path.toLocalFile()});

    connect(appProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError err) {
        qDebug() << "QPROCESS ERROR" << err;
        emit this->progressFinished();

    });
    connect(appProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << "QPROCESS FINISHED" << exitCode << exitStatus;
        emit this->progressFinished();
    });
}

void Package::updatePackage()
{

}

void Package::removePackage()
{

}

void Package::installPackage()
{
    const auto package =   this->m_data->downloads.at(this->m_packageIndex);
    auto downloader = new FMH::Downloader;

    qDebug() << "TRY TO DOWNLOAD LINK" << this->m_link;

    auto appimagePath = NX::AppsPath.toString()+("/")+package->name;

    connect(downloader, &FMH::Downloader::progress, this, &Package::setProgress);
    connect(downloader, &FMH::Downloader::done, [this, downloader, appimagePath]()
    {
        this->integratePackage(appimagePath);
        downloader->deleteLater();
    });

    downloader->downloadFile(this->m_link, appimagePath);
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

void Package::setPath(const QString &path)
{
    m_path = QUrl(path);
    emit pathChanged(m_path);

}
