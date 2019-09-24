#ifndef STORES_RESPONSEDTO_APPLICATION_H
#define STORES_RESPONSEDTO_APPLICATION_H

#include <QObject>
#include <QString>

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

    QString toString() {
      return "{name: " + name + ", link: " + link + ", ...}";
    }
  };

  class PreviewPic {
   public:
    QString pic = "";
    QString smallPic = "";
    QString toString() {
      return "{pic: " + pic + ", smallpic: " + smallPic + "}";
    }
  };

  class PreviewUrl {
   public:
    QString preview = "";

    QString toString() { return "{preview: " + preview + "}"; }
  };

  Application(QObject* parent = nullptr) : QObject(parent) {}

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
  QList<PreviewPic*> previewPics = QList<PreviewPic*>();
  QList<PreviewUrl*> previewUrls = QList<PreviewUrl*>();
  QList<Download*> downloads = QList<Download*>();

  QString toString(bool verbose = false) {
    QString q;
    if (!verbose) {
      q = "{name: " + name + ", ...}";
    } else {
      q = "{id: " + id + ", name: " + name + ", changed: " + changed +
          ", comments: " + comments + ", created: " + created +
          ", description: " + description + ", detailpage: " + detailPage +
          ", details: " + details +
          ", totalDownloads: " + QString::number(totalDownloads) +
          ", ghnsExcluded: " + ghnsExcluded + ", language: " + language +
          ", personId: " + personId + ", score: " + QString::number(score) +
          ", summary: " + summary + ", tags: " + tags + ", typeId: " + typeId +
          ", typeName: " + typeName + ", version: " + version +
          ", xdgType: " + xdgType + "}";
    }

    for (const auto& i : downloads) {
      q.append("\n            - " + i->toString());
    }

    q.append("\n");

    for (const auto& i : previewPics) {
      q.append("\n            - " + i->toString());
    }

    q.append("\n");

    for (const auto& i : previewUrls) {
      q.append("\n            - " + i->toString());
    }

    q.append("\n");

    return q;
  }
};

#endif
