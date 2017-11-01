#include <QTest>
#include <QSignalSpy>
#include <QDebug>

#include "../entities/app.h"
#include "../entities/change.h"
#include "../entities/release.h"
#include "../entities/repository.h"
#include "../entities/registry.h"
#include "../entities/progresslistener.h"

#include "../gateways/kf5downloadmanager.h"

#include "dummychange.h"
#include "dummyinstallchange.h"
#include "dummyrelease.h"
#include "dummyrepository.h"

class DummyProgressListener: public ProgressListener
{
public:
    DummyProgressListener() {}

    virtual void progress(const int progress, const int total, const QString statusMessage) {
//        qDebug() << statusMessage << " "<< progress << " of "<< total;
        m_progress = true;
    }
    virtual void error(const QString &errorMessage) {
//        qDebug() << errorMessage;
        m_error = true;
        m_errorMessage = errorMessage;
    }
    virtual void finished() {
//        qDebug() << "Finished";
        m_finish = true;
    }

    bool m_progress = false;
    bool m_finish = false;
    bool m_error = false;
    QString m_errorMessage;
};

class GatewaysTests : public QObject
{
    Q_OBJECT
private slots:

    void testKF5DownloadManager() {
        KF5DownloadManager downloadManager;
        DummyProgressListener listener;

        QString filePath = "/tmp/testDownloader";
        downloadManager.download("https://www.google.com", filePath, &listener);
        qDebug() << filePath;
        QFile file(filePath);
        QVERIFY(file.exists());
        QVERIFY(listener.m_progress);
        QVERIFY(listener.m_finish);
        QVERIFY(!listener.m_error);

        file.remove();
    }
};
QTEST_MAIN(GatewaysTests)
#include "gateways_tests.moc"
