#ifndef STORES_RESPONSEDTO_APPLICATION_H
#define STORES_RESPONSEDTO_APPLICATION_H

#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QUrl>

#include <MauiKit/FileBrowsing/downloader.h>

#include "category.h"

class Application : public QObject {
  Q_OBJECT

public:
  class Download {
  public:
    QString packageArch = "";
    QString packageType = "";
    QString gpgFingerprint = "";
    QString gpgSignature = "";
    QString link = "";
    QString name = "";
    QString packageName = "";
    QString price = "";
    QString repository = "";
    QString size = "";
    QString tags = "";
    QString type = "";
    QString way = "";

    QString toString();
    FMH::Downloader *downloadFile(QUrl filepath);
  };

  class PreviewPic {
  public:
    QString pic = "";
    QString smallPic = "";
    QString toString();
  };

  class PreviewUrl {
  public:
    QString preview = "";

    QString toString();
  };

  Application(QObject *parent = nullptr) : QObject(parent) {}

  QString id = "";
  QString name = "";
  QString changed = "";
  QString comments = "";
  QString created = "";
  QString description = "";
  QString detailPage = "";
  QString details = "";
  double totalDownloads = 0;
  QString ghnsExcluded = "";
  QString language = "";
  QString personId = "";
  double score = 0;
  QString summary = "";
  QString tags = "";
  QString typeId = "";
  QString typeName = "";
  QString version = "";
  QString xdgType = "";
  QList<PreviewPic *> previewPics = QList<PreviewPic *>();
  QList<PreviewUrl *> previewUrls = QList<PreviewUrl *>();
  QList<Download *> downloads = QList<Download *>();

  Category::CategoryStore applicationStore;

  QString toString(bool verbose = false);
};

#endif
