#include <QTest>

#include <QDataStream>
#include "../appimage/appimagedownloadjob.h"
#include "../appimage/appimageuninstalljob.h"

class AppImageTest : public QObject {
    Q_OBJECT
private slots:
    void testAppImageDownload() {
        AppImageDownloadJob job("https://github.com/AppImage/AppImageUpdate/releases/download/continuous/AppImageUpdate-x86_64.AppImage");

        connect(&job, &AppImageDownloadJob::statusChanged, [] (QString status) {
            qDebug() << status;
        });

        job.exec();
    }

    void testAppImageUninstall() {
        AppImageUninstallJob job("subsurface");

        connect(&job, &AppImageUninstallJob::statusChanged, [] (QString status) {
            qDebug() << status;
        });

        job.exec();

    }
};
QTEST_MAIN(AppImageTest)
#include "appimage_test.moc"
