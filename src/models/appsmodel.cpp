#include "appsmodel.h"

#include <QDebug>
#include <QRegularExpression>
#include <QPixmap>
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

                KFileItem kAppimageItem(QUrl("file://" + QDir::homePath() + "/Applications/" + entry.stringValue(entry.UDS_NAME)));;

                this->m_list.append(FMH::MODEL{
                    {FMH::MODEL_KEY::NAME, entry.stringValue(entry.UDS_NAME)},
                    {FMH::MODEL_KEY::SMALL_PIC, kAppimageItem.iconName()}
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

