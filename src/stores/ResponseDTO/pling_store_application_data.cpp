#include "pling_store_application_data.h"

#include <QNetworkReply>
#include <QNetworkRequest>

QString PlingStoreApplicationData::toString(bool verbose) {
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
        ", summary: " + summary + ", tags: " + tags +
        ", typeId: " + typeId + ", typeName: " + typeName +
        ", version: " + version + ", xdgType: " + xdgType + "}";
  }

  for (const auto &i : downloads) {
    q.append("\n            - " + i->toString());
  }

  q.append("\n");

  for (const auto &i : previewPics) {
    q.append("\n            - " + i->toString());
  }

  q.append("\n");

  for (const auto &i : previewUrls) {
    q.append("\n            - " + i->toString());
  }

  q.append("\n");

  return q;
}

QString PlingStoreApplicationData::PreviewUrl::toString() { return "{preview: " + preview + "}"; }

QString PlingStoreApplicationData::PreviewPic::toString() {
    return "{pic: " + pic + ", smallpic: " + smallPic + "}";
}

QString PlingStoreApplicationData::Download::toString() {
    return "{name: " + name + ", link: " + link + ", ...}";
}
