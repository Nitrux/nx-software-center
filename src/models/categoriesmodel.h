#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QObject>

#include <MauiKit/Core/fmh.h>
#include <MauiKit/Core/mauilist.h>

#include "ResponseDTO/category.h"
#include "categorymanager.h"

class Store;
class CategoriesModel : public MauiList
{
    Q_OBJECT
    Q_PROPERTY(Category * category READ category WRITE setCategory NOTIFY categoryChanged)

public:
    CategoriesModel(QObject * parent = nullptr);

    // QQmlParserStatus interface
    void componentComplete() override final;

    // MauiList interface
    const FMH::MODEL_LIST &items() const override final;

    Category * category() const;

private:
    FMH::MODEL_LIST m_list;
    QHash<QString, Category *> m_categoryMap; //id,app
    Store *m_store;

    Category * m_category;
    CategoryManager *m_categoryManager;

    void setList();

public slots:
    Category *baseCategory();
    Category *audioCategory();
    Category *videoCategory();
    Category *educationCategory();
    Category *graphicsCategory();
    Category *gamesCategory();

    Category *getCategory(const QString &id);

    void setCategory(Category * category);
    void clear();

signals:
    void categoryChanged();
};

#endif // CATEGORIESMODEL_H
