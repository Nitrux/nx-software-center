#ifndef STORES_RESPONSEDTO_CATEGORY_H
#define STORES_RESPONSEDTO_CATEGORY_H

#include <QObject>

class Category : public QObject {
  Q_OBJECT
 public:
  Category(QObject* parent = nullptr);

  QString id = "";
  QString parentId = "";
  QString name = "";
  QString displayName = "";
  QString xdgType = "";
  QList<Category*> categories = QList<Category*>();

  bool isAParent();
  double childCount();
  QString toString();
};

#endif
