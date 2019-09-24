#ifndef STORES_RESPONSEDTO_CATEGORYRESPONSEDTO_H
#define STORES_RESPONSEDTO_CATEGORYRESPONSEDTO_H

#include "category.h"

#include <QList>
#include <QObject>
#include <QString>

class CategoryResponseDTO : public QObject {
  Q_OBJECT

 public:
  CategoryResponseDTO(QObject* parent = nullptr) : QObject(parent) {}

  QList<Category*> categories;
  QString message;
  QString status;
  double statusCode;
  double totalItems;

  QString toString() {
    QString r = "{categoriesCount: " + QString::number(categories.size()) +
                ", message: " + message + ", status: " + status +
                ", statusCode: " + QString::number(statusCode) +
                ", totalItems: " + QString::number(totalItems) + "}\n\n";

    for (Category* c : categories) {
      r.append(c->toString() + "\n");

      for (Category* cc : c->categories) {
        r.append("    " + cc->toString() + "\n");
      }

      r.append("\n");
    }

    return r;
  }
};

#endif
