#include "categoriesmodel.h"
#include "store.h"
#include "appimagehubstore.h"
#include "ResponseDTO/category.h"

CategoriesModel::CategoriesModel(QObject *parent) : MauiList(parent),
    m_store(new AppImageHubStore(this)), m_currentCategory(nullptr), m_category(nullptr)
{
    connect(this, &CategoriesModel::categoryChanged, this, &CategoriesModel::setList);
}

static const QHash<QString, QString> iconName =
{
    {"All","appimage-store"},
    {"Programming","qrc:/applications-development.svg"},
    {"System & Tools", "qrc:/applications-utilities.svg"},
    {"Audio", "qrc:/applications-multimedia.svg"},
    {"Office", "qrc:/applications-office.svg"},
    {"Video", "qrc:/applications-multimedia.svg"},
    {"Internet", "qrc:/applications-internet.svg"},
    {"Graphics", "qrc:/applications-graphics.svg"},
    {"Education", "qrc:/applications-other.svg"},
    {"Games", "qrc:/applications-games.svg"},
};

static const QHash<QString, QString> cardColor =
{
    {"All","appimage-store"},
    {"Programming","qrc:/applications-development.svg"},
    {"System & Tools", "qrc:/applications-utilities.svg"},
    {"Audio", "qrc:/applications-multimedia.svg"},
    {"Office", "qrc:/applications-office.svg"},
    {"Video", "qrc:/applications-multimedia.svg"},
    {"Internet", "qrc:/applications-internet.svg"},
    {"Graphics", "qrc:/applications-graphics.svg"},
    {"Education", "qrc:/applications-other.svg"},
    {"Games", "qrc:/applications-games.svg"},
};

void CategoriesModel::componentComplete()
{
    setList();
}

const FMH::MODEL_LIST &CategoriesModel::items() const
{
    return this->m_list;
}

Category *CategoriesModel::getCurrentCategory() const
{
    return m_currentCategory;
}

Category * CategoriesModel::category() const
{
    return m_category;
}

void CategoriesModel::setList()
{
    this->clear();

    if(m_category)
    {
        emit this->preListChanged();

        for(auto &c : m_category->categories)
        {
            qDebug() << c->toString();
            this->m_list << FMH::MODEL { {FMH::MODEL_KEY::ID, c->id},
            {FMH::MODEL_KEY::NAME, c->name},
            {FMH::MODEL_KEY::ICON, iconName[c->name]},
            {FMH::MODEL_KEY::CATEGORY, tr("Apps")},
            {FMH::MODEL_KEY::TITLE, c->displayName},
            {FMH::MODEL_KEY::PARENT_ID, c->parentId},
            //            {FMH::MODEL_KEY::COUNT, QString::number(c->childCount())},
        };
        this->m_categoryMap.insert(c->id, std::move(c));
    }

    emit this->postListChanged();

}else
{
this->m_store->getCategories();
connect(this->m_store, &Store::categoriesResponseReady, [=](CategoryResponseDTO *response)
{
    if(m_category)
        return;

    emit this->preListChanged();
    this->m_list.append({{FMH::MODEL_KEY::TITLE, tr("All")},
                         {FMH::MODEL_KEY::NAME, "all"},
                         {FMH::MODEL_KEY::ID, "0"},
                         {FMH::MODEL_KEY::ICON, iconName["All"]},
                         {FMH::MODEL_KEY::CATEGORY, tr("Apps")}});

    const auto categories = response->categories;
    for(auto &c : response->categories)
    {
        qDebug() << c->toString();
        this->m_list << FMH::MODEL { {FMH::MODEL_KEY::ID, c->id},
        {FMH::MODEL_KEY::NAME, c->name},
        {FMH::MODEL_KEY::ICON, iconName[c->name]},
        {FMH::MODEL_KEY::CATEGORY, tr("Apps")},
        {FMH::MODEL_KEY::TITLE, c->displayName},
        {FMH::MODEL_KEY::PARENT_ID, c->parentId},
        //            {FMH::MODEL_KEY::COUNT, QString::number(c->childCount())},
    };
    this->m_categoryMap.insert(c->id, std::move(c));
}

qDebug()<< this->m_list;
emit this->postListChanged();

});
}
}

void CategoriesModel::setCurrentCategory(const QString &id)
{
    const auto category =  this->m_categoryMap[id];

    if(category)
    {
        this->m_currentCategory = category;
    }
    else
    {
        this->m_currentCategory = baseCategory ();
    }
    emit this->currentCategoryChanged(this->m_currentCategory);
}

Category * CategoriesModel::baseCategory()
{
    auto category = new Category(this);
    category->id = "0";
    category->displayName = "All";
    category->name = "all";

    return category;
}

Category * CategoriesModel::featureCategory()
{
    return baseCategory ();
}

Category *CategoriesModel::audioCategory()
{
    auto category = new Category(this);
    category->id = "392";
    category->displayName = "Audio";
    category->name = "audio";
    return category;
}

Category *CategoriesModel::videoCategory()
{
    auto category = new Category(this);
    category->id = "481";
    category->displayName = "Video";
    category->name = "video";
    return category;
}

Category *CategoriesModel::educationCategory()
{
    auto category = new Category(this);
    category->id = "544";
    category->displayName = "Education";
    category->name = "education";
    return category;
}

Category *CategoriesModel::graphicsCategory()
{
    auto category = new Category(this);
    category->id = "526";
    category->displayName = "Graphics";
    category->name = "graphics";
    return category;
}

Category *CategoriesModel::gamesCategory()
{
    auto category = new Category(this);
    category->id = "6";
    category->displayName = "Games";
    category->name = "games";
    return category;
}

Category *CategoriesModel::getCategory(const QString &id)
{
    auto index = this->indexOf(FMH::MODEL_KEY::ID, id);
    if(index >=0 )
    {
        return this->m_categoryMap[id];
    }

    return nullptr;
}

void CategoriesModel::setCategory(Category * category)
{
    if (m_category == category)
        return;

    m_category = category;
    emit categoryChanged();
}

void CategoriesModel::clear()
{
    emit this->preListChanged();
    this->m_list.clear();
    this->m_categoryMap.clear();
    emit this->postListChanged();
}

