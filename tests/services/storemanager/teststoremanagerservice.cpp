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
    
    QList<QVariant> arguments = spyGetCategories.takeFirst();
    CategoryResponseDTO *result = qvariant_cast<CategoryResponseDTO*>(arguments.at(0));

    QVERIFY(result->categories.size() > 0);
}

void TestStoreManagerService::testGetApplications()
{
    QSignalSpy spyGetApplications(_storeManagerService, SIGNAL(applicationsResponseReady(ApplicationResponseDTO *)));

    const auto categoryId = "0";
    Category *category = new Category();
    _storeManagerService->getApplications({categoryId}, "", Store::SORT_MODE::MODE_NEWEST, "1", "10", {}, Store::Arch::all, category);

    spyGetApplications.wait();

    QCOMPARE(spyGetApplications.length(), 1);

    QList<QVariant> arguments = spyGetApplications.takeFirst();
    ApplicationResponseDTO *result = qvariant_cast<ApplicationResponseDTO*>(arguments.at(0));

    QVERIFY(result->applications.size() > 0);
    QVERIFY(result->applications.size() <= 10);
}

void TestStoreManagerService::cleanupTestCase()
{
    delete _storeManagerService;
}

QTEST_MAIN(TestStoreManagerService);