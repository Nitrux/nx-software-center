#include "teststoremanagerservice.h"

// libraries
#include <QTest>

void TestStoreManagerService::initTestCase()
{
    _storeManagerService = new StoreManagerService(this);

    qRegisterMetaType<CategoryResponseDTO *>("CategoryResponseDTO *");
    qRegisterMetaType<ApplicationResponseDTO *>("ApplicationResponseDTO *");
}

void TestStoreManagerService::testGetCategories()
{
    QSignalSpy spyGetCategories(_storeManagerService, SIGNAL(categoriesResponseReady(CategoryResponseDTO *)));

    _storeManagerService->getCategories();

    spyGetCategories.wait();

    QCOMPARE(spyGetCategories.length(), 1);
    
    auto arguments = spyGetCategories.takeFirst();
    const auto &categoryResult = qvariant_cast<CategoryResponseDTO*>(arguments.at(0));

    QVERIFY(categoryResult->categories.size() > 0);
}

void TestStoreManagerService::testGetApplications()
{
    QSignalSpy spyGetApplications(_storeManagerService, SIGNAL(applicationsResponseReady(ApplicationResponseDTO *)));

    const auto categoryId = "0";
    auto *category = new Category();
    _storeManagerService->getApplications({categoryId}, "", Store::SORT_MODE::MODE_NEWEST, "1", "10", {}, Store::Arch::all, category);

    spyGetApplications.wait();

    QCOMPARE(spyGetApplications.length(), 1);

    auto arguments = spyGetApplications.takeFirst();
    const auto &applicationResult = qvariant_cast<ApplicationResponseDTO*>(arguments.at(0));

    QVERIFY(applicationResult->applications.size() > 0);
    QVERIFY(applicationResult->applications.size() <= 10);
}

void TestStoreManagerService::testGetApprepoApplications()
{
    QSignalSpy spyGetApprepoApplications(_storeManagerService, SIGNAL(applicationsResponseReady(ApplicationResponseDTO *)));

    const auto categoryId = "0";
    auto *category = new Category();
    category->id = "1";
    category->categoryStore = Category::CategoryStore::APPREPO;

    _storeManagerService->getApplications({categoryId}, "", Store::SORT_MODE::MODE_NEWEST, "", "", {}, Store::Arch::all, category);

    spyGetApprepoApplications.wait();

    QCOMPARE(spyGetApprepoApplications.length(), 1);

    auto arguments = spyGetApprepoApplications.takeFirst();
    const auto &applicationResult = qvariant_cast<ApplicationResponseDTO*>(arguments.at(0));

    QVERIFY(applicationResult->applications.size() > 0);
}

void TestStoreManagerService::testGetAppimagehubApplications()
{
    QSignalSpy spyGetAppimagehubApplications(_storeManagerService, SIGNAL(applicationsResponseReady(ApplicationResponseDTO *)));

    const auto categoryId = "0";
    auto *category = new Category();
    category->categoryStore = Category::CategoryStore::APPIMAGEHUB;

    _storeManagerService->getApplications({categoryId}, "", Store::SORT_MODE::MODE_NEWEST, "1", "10", {}, Store::Arch::all, category);

    spyGetAppimagehubApplications.wait();

    QCOMPARE(spyGetAppimagehubApplications.length(), 1);

    auto arguments = spyGetAppimagehubApplications.takeFirst();
    const auto &applicationResult = qvariant_cast<ApplicationResponseDTO*>(arguments.at(0));

    QVERIFY(applicationResult->applications.size() > 0);
    QVERIFY(applicationResult->applications.size() <= 10);
}

void TestStoreManagerService::cleanupTestCase()
{
    delete _storeManagerService;
}

QTEST_MAIN(TestStoreManagerService);