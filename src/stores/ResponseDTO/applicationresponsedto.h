#ifndef STORES_RESPONSEDTO_APPLICATIONRESPONSEDTO_H
#define STORES_RESPONSEDTO_APPLICATIONRESPONSEDTO_H

#include "application.h"

#include <QList>
#include <QObject>

class ApplicationResponseDTO : public QObject {
  Q_OBJECT

 public:
  ApplicationResponseDTO(QObject* parent = nullptr) : QObject(parent) {}

  QList<Application*> applications;
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

    for (Application* a : applications) {
      r.append("      - " + a->toString() + "\n");
    }

    return r;
  }
};

#endif
