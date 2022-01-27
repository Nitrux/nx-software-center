#include "appimagetools.h"

#include <QDebug>
#include <QSettings>
#include <QTemporaryFile>

#include <MauiKit/FileBrowsing/fmstatic.h>

#include <appimage/core/AppImage.h>
#include <appimage/desktop_integration/IntegrationManager.h>
#include <appimage/utils/ResourcesExtractor.h>

AppImageTools::AppImageTools(QObject *parent)
    : QObject(parent)
{
}

void AppImageTools::integrate(const QUrl &url)
{
    try {
        const auto &localFilePath = url.toLocalFile();
        appimage::core::AppImage app(localFilePath.toStdString());
        appimage::desktop_integration::IntegrationManager manager;
        if (!manager.isARegisteredAppImage(localFilePath.toStdString())) {
            if (manager.shallAppImageBeRegistered(app)) {
                manager.registerAppImage(app);
            }
        }

    } catch (const appimage::core::AppImageError &err) {
        qWarning() << "could not integrate appimage:" << err.what();
    }
}

void AppImageTools::unintegrate(const QUrl &url)
{
    try {
        const auto &localFilePath = url.toLocalFile();
        appimage::desktop_integration::IntegrationManager manager;
        manager.unregisterAppImage(localFilePath.toStdString());
    } catch (const appimage::core::AppImageError &err) {
        qWarning() << "could not remove appimage desktop integration" << err.what();
    }
}

const FMH::MODEL AppImageTools::desktopData(const QUrl &url)
{
    FMH::MODEL res = FMStatic::getFileInfoModel(url);
    try {
        appimage::core::AppImage app(url.toLocalFile().toUtf8().constData());
        appimage::utils::ResourcesExtractor extractor(app);
        auto data = extractor.extractText(extractor.getDesktopEntryPath());
        qDebug() << "Desktop file" << QString::fromStdString(data);

        QTemporaryFile tmpFile;
        if (!tmpFile.open()) {
            return res;
        }

        tmpFile.write(QByteArray::fromStdString(data));
        const QString fileName = tmpFile.fileName();
        tmpFile.close();
        QSettings desktopFile(fileName, QSettings::IniFormat);
        desktopFile.beginGroup("Desktop Entry");
        QString name = desktopFile.value("Name").toString();
        QString categories = desktopFile.value("Categories").toString();
        desktopFile.endGroup();

        res[FMH::MODEL_KEY::LABEL] = name;
        res[FMH::MODEL_KEY::CATEGORY] = categories;

    } catch (...) {
        qDebug() << "could not get data from appimage";
    }

    return res;
}
