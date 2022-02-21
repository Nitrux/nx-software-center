#ifndef STORES_RESPONSEDTO_APPLICATIONRESPONSEDTO_H
#define STORES_RESPONSEDTO_APPLICATIONRESPONSEDTO_H

#include <QList>
#include <QObject>

#include "pling_store_application_data.h"

class ApplicationResponseDTO : public QObject {
  Q_OBJECT

public:
  ApplicationResponseDTO(QObject *parent = nullptr) : QObject(parent) {}

  QList<PlingStoreApplicationData *> applications;
  QString message;
  QString status;
  QString totalItems;
  double itemsPerPage;
  double statusCode;

  QString toString() {
    QString r = "* {applicationCount: " + QString::number(applications.size()) +
                ", message: " + message + ", status: " + status +
                ", itemsPerPage: " + QString::number(itemsPerPage) +
                ", statusCode: " + QString::number(statusCode) +
                ", totalItems: " + totalItems + "}\n";

    for (PlingStoreApplicationData *a : applications) {
      r.append("      - " + a->toString() + "\n");
    }

    return r;
  }
};

#endif
