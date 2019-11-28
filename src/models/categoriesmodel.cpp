#include "categoriesmodel.h"
#include "store.h"
#include "appimagehubstore.h"

CategoriesModel::CategoriesModel(QObject *parent) : MauiList(parent),
    m_store(new AppImageHubStore(this)) {}

static const QHash<QString, QString> iconName =
{
    {"All","appimage-store"},
    {"Programming","applications-development"},
    {"System & Tools", "applications-utilities"},
    {"Audio", "applications-multimedia"},
    {"Office", "applications-office"},
    {"Video", "applications-multimedia"},
    {"Internet", "applications-internet"},
    {"Graphics", "applications-graphics"},
    {"Education", "applications-other"},
    {"Games", "applications-games"},
};

void CategoriesModel::componentComplete()
{
    this->m_store->getCategories();
    connect(this->m_store, &Store::categoriesResponseReady, [=](CategoryResponseDTO *response)
    {
        emit this->preListChanged();
        this->m_list.append({{FMH::MODEL_KEY::TITLE, tr("All")},
                             {FMH::MODEL_KEY::ICON, iconName["All"]},
                             {FMH::MODEL_KEY::CATEGORY, tr("Apps")}});

        const auto categories = response->categories;
        for(const auto &c : categories)
        {
            this->m_list << FMH::MODEL { {FMH::MODEL_KEY::ID, c->id},
            {FMH::MODEL_KEY::NAME, c->name},
            {FMH::MODEL_KEY::ICON, iconName[c->name]},
            {FMH::MODEL_KEY::CATEGORY, tr("Apps")},
            {FMH::MODEL_KEY::TITLE, c->displayName},
            {FMH::MODEL_KEY::PARENT_ID, c->parentId},
            {FMH::MODEL_KEY::COUNT, QString::number(c->childCount())},
        };
    }
    emit this->postListChanged();
});
}

FMH::MODEL_LIST CategoriesModel::items() const
{
    return this->m_list;
}

