#include <QTest>
#include <QSignalSpy>

#include <QDataStream>
#include "../appimage/appimage.h"
#include "../appimage/appimagedownloadjob.h"
#include "../appimage/appimageuninstalljob.h"
#include "../appimage/appimagehubrepository.h"

class AppImageTest : public QObject
{
    Q_OBJECT
private slots:
    void testAppImageHubRepository()
    {
        AppImageHubRepository repository;
        repository.update();

        QSignalSpy spy(&repository, &AppImageHubRepository::itemsChanged);
        spy.wait();

//        for (AppImage * appImage: repository.items())
//        {
//            qDebug() << "----------------------";
//            qDebug() << appImage->id();
//            qDebug() << appImage->description();
//            qDebug() << appImage->licence();
//            qDebug() << appImage->authors();
//            qDebug() << appImage->links();
//            qDebug() << appImage->screenshots();
//        }
        if (repository.count() > 0)
        {
            AppImage *image = repository.items().first();
            repository.findDownloadLinks(image, "x86_64");

            QSignalSpy spy(image, &AppImage::linksChanged);
            spy.wait();
        }
    }
//    void testAppImageDownload()
//    {
//        AppImageDownloadJob job("https://github.com/AppImage/AppImageUpdate/releases/download/continuous/AppImageUpdate-x86_64.AppImage");

//        connect(&job, &AppImageDownloadJob::statusChanged, [] (QString status)
//        {
//            qDebug() << status;
//        });

//        job.exec();
//    }

//    void testAppImageUninstall()
//    {
//        AppImageUninstallJob job("subsurface");

//        connect(&job, &AppImageUninstallJob::statusChanged, [] (QString status)
//        {
//            qDebug() << status;
//        });

//        job.exec();
//    }
};
QTEST_MAIN(AppImageTest)
#include "appimage_test.moc"
