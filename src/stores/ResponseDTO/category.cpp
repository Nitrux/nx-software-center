#include "category.h"

Category::Category(QObject* parent) : QObject(parent) {}

bool Category::isAParent() { return parentId != ""; }

double Category::childCount() { return categories.count(); }

QString Category::toString() {
  return "{id: " + id + ", parentId: " + parentId + ", name: " + name +
         ", displayName: " + displayName + ", xdgType: " + xdgType +
         ", children: " + QString::number(categories.count()) + "}";
}
