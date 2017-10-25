#include "dummyrepository.h"

#include "../entities/app.h"

DummyRepository::DummyRepository(): Repository()
{

}

QString DummyRepository::id()
{
    return "DummyRepository";
}

void DummyRepository::updateCache()
{
    apps.clear();

    apps.insert("app_1", new App("app_1"));
    apps.insert("app_2", new App("app_2"));
    apps.insert("app_3", new App("app_3"));
    apps.insert("app_4", new App("app_4"));
    apps.insert("app_5", new App("app_5"));

}
