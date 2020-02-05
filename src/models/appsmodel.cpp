#include "appsmodel.h"

#include <QDebug>
#include <QRegularExpression>
#include <QPixmap>
#include <QProcess>
#include <KIO/ListJob>
#include <KIO/PreviewJob>
#include <KFileItem>
#include <KIO/ThumbCreator>

AppsModel::AppsModel(QObject *parent) : MauiList(parent) {}

void AppsModel::componentComplete() {
    QRegularExpression fileNameRe("\\.appimage", QRegularExpression::CaseInsensitiveOption);
    KIO::ListJob *listJob = KIO::listDir(QUrl("file://" + QDir::homePath() + "/Applications"));

    connect(listJob, &KIO::ListJob::entries, [=](KIO::Job *job, const KIO::UDSEntryList &list) {
        Q_UNUSED(job)

        for (KIO::UDSEntry entry : list) {
            if (fileNameRe.match(entry.stringValue(entry.UDS_NAME)).hasMatch()) {
                emit this->preItemAppended();

                QString filePath("file://" + QDir::homePath() + "/Applications/" + entry.stringValue(entry.UDS_NAME));
                KFileItem kAppimageItem(filePath);

                this->m_list.append(FMH::MODEL{
                    {FMH::MODEL_KEY::NAME, entry.stringValue(entry.UDS_NAME)},
                    {FMH::MODEL_KEY::SMALL_PIC, kAppimageItem.iconName()},
                    {FMH::MODEL_KEY::PATH, filePath}
                });

                emit this->postItemAppended();
            }
        }
    });

    connect(listJob, &KIO::ListJob::slotError, [=](int i, QString err) {
        Q_UNUSED(i)
        qDebug() << "INSTALLED APP LISTING ERROR" << err;
    });
}

FMH::MODEL_LIST AppsModel::items() const { return this->m_list; }

void AppsModel::launchApp(QString path)
{
    QProcess *appProcess = new QProcess(this);
    appProcess->start(path.replace("file://", ""));

//    appProcess->waitForFinished();

//    qDebug() << appProcess->readAll();

    connect(appProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError err) {
        qDebug() << "QPROCESS ERROR" << err;
        emit appLaunchError(err);
    });
    connect(appProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << "QPROCESS FINISHED" << exitCode << exitStatus;
        emit appLaunchSuccess();
    });
}

void AppsModel::removeApp(QString path)
{
}

