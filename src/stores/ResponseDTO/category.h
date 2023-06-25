#ifndef STORES_RESPONSEDTO_CATEGORY_H
#define STORES_RESPONSEDTO_CATEGORY_H

#include <QObject>

class Category : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER name CONSTANT FINAL)
    Q_PROPERTY(QString id MEMBER id CONSTANT FINAL)
    Q_PROPERTY(QString displayName MEMBER displayName CONSTANT FINAL)
    Q_PROPERTY(CategoryStore categoryStore MEMBER categoryStore CONSTANT FINAL)

public:
    Category(QObject* parent = nullptr);

    enum CategoryStore {
        APPIMAGEHUB = 0
    };
    Q_ENUM(CategoryStore);

    QString id = "";
    QString parentId = "";
    QString name = "";
    QString displayName = "";
    QString xdgType = "";
    CategoryStore categoryStore;
    QList<Category*> categories = QList<Category*>();
    

    bool isAParent();
    double childCount();
    QString toString();
};

#endif
