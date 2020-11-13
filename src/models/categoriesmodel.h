#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QObject>

#ifdef STATIC_MAUIKIT
#include "fmh.h"
#include "mauilist.h"
#else
#include <MauiKit/fmh.h>
#include <MauiKit/mauilist.h>
#endif

class Store;
class Category;
class CategoriesModel : public MauiList
{
		Q_OBJECT
		Q_PROPERTY(Category * currentCategory READ getCurrentCategory NOTIFY currentCategoryChanged)

		Q_PROPERTY(Category * category READ getCategory WRITE setCategory NOTIFY categoryChanged)
	public:
		CategoriesModel(QObject * parent = nullptr);

		// QQmlParserStatus interface
		void componentComplete() override final;

		// MauiList interface
		FMH::MODEL_LIST items() const override final;

		Category * getCurrentCategory() const;

		Category * getCategory() const;

	private:
		FMH::MODEL_LIST m_list;
		QHash<QString, Category *> m_categoryMap; //id,app
		Store *m_store;

		Category * m_currentCategory;
		Category * m_category;

		void setList();

	public slots:
		void setCurrentCategory(const QString &id);
		Category *baseCategory();
		Category *featureCategory();

		void setCategory(Category * category);
		void clear();

	signals:
		void currentCategoryChanged(Category * currentCategory);
		void categoryChanged();
};

#endif // CATEGORIESMODEL_H
