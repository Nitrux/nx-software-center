#include "categoriesmodel.h"
#include "store.h"
#include "appimagehubstore.h"

CategoriesModel::CategoriesModel(QObject *parent) : MauiList(parent),
    m_store(new AppImageHubStore(this)), m_category(nullptr)
{
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
    {"Programming","#311b92"},
    {"System & Tools", "#536dfe"},
    {"Audio", "#ff4081"},
    {"Office", "#00b0ff"},
    {"Video", "#1de9b6"},
    {"Internet", "#ffff00"},
    {"Graphics", "#ff6e40"},
    {"Education", "#455a64"},
    {"Games", "#c6ff00"},
};

void CategoriesModel::componentComplete()
{
    connect(this, &CategoriesModel::categoryChanged, this, &CategoriesModel::setList);
    setList();
}

const FMH::MODEL_LIST &CategoriesModel::items() const
{
    return this->m_list;
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
        qDebug() << "Setting SUBCATEGORIES list" << m_category->categories.size();

        for(auto c : m_category->categories)
        {

            qDebug() << c->toString();
            this->m_list << FMH::MODEL { {FMH::MODEL_KEY::ID, c->id},
            {FMH::MODEL_KEY::NAME, c->name},
            {FMH::MODEL_KEY::ICON, iconName[c->name]},
            {FMH::MODEL_KEY::COLOR, cardColor[c->name]},
            {FMH::MODEL_KEY::CATEGORY, tr("Apps")},
            {FMH::MODEL_KEY::TITLE, c->displayName},
            {FMH::MODEL_KEY::PARENT_ID, c->parentId},
            //            {FMH::MODEL_KEY::COUNT, QString::number(c->childCount())},
        };

        Category * cat = new Category(this);
        cat->id = c->id;
        cat->parentId = c->parentId;
        cat->name = c->name;
        cat->displayName = c->displayName;
        cat->xdgType = c->xdgType;
        cat->categories = c->categories;

        this->m_categoryMap.insert(c->id, cat);
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
//    this->m_list.append({{FMH::MODEL_KEY::TITLE, tr("All")},
//                         {FMH::MODEL_KEY::NAME, "all"},
//                         {FMH::MODEL_KEY::ID, "0"},
//                         {FMH::MODEL_KEY::ICON, iconName["All"]},
//                         {FMH::MODEL_KEY::CATEGORY, tr("Apps")}});

    for(auto c : response->categories)
    {
        qDebug() << c->toString();
        this->m_list << FMH::MODEL { {FMH::MODEL_KEY::ID, c->id},
        {FMH::MODEL_KEY::NAME, c->name},
        {FMH::MODEL_KEY::ICON, iconName[c->name]},
        {FMH::MODEL_KEY::COLOR, cardColor[c->name]},
        {FMH::MODEL_KEY::CATEGORY, tr("Apps")},
        {FMH::MODEL_KEY::TITLE, c->displayName},
        {FMH::MODEL_KEY::PARENT_ID, c->parentId},
        //            {FMH::MODEL_KEY::COUNT, QString::number(c->childCount())},
    };
//    auto cat = std::move(c);
//    auto _cat = this->m_categoryMap[id];

        Category * cat = new Category(this); // copy prtoblem here so do a deep copy of the category to store
        cat->id = c->id;
        cat->parentId = c->parentId;
        cat->name = c->name;
        cat->displayName = c->displayName;
        cat->xdgType = c->xdgType;
        cat->categories = c->categories;
//        qDebug() << "requesting category" << _cat->categories.size();

    this->m_categoryMap.insert(c->id, cat);
}

qDebug()<< this->m_list;
emit this->postListChanged();

});
}
}

Category * CategoriesModel::baseCategory()
{
    auto category = new Category(this);
    category->id = "0";
    category->displayName = "All";
    category->name = "all";

    return category;
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
    qDebug() << "requesting category" << id;

    if(this->m_categoryMap.contains(id))
    {
        return this->m_categoryMap[id];
    }

    return this->baseCategory();
}

void CategoriesModel::setCategory(Category * category)
{
    if (category == nullptr)
        return;

    m_category = category;
    qDebug() << "SETTING UP CATEGORY" << m_category->name;

    emit categoryChanged();
}

void CategoriesModel::clear()
{
    emit this->preListChanged();
    this->m_list.clear();
    this->m_categoryMap.clear();
    emit this->postListChanged();
}

