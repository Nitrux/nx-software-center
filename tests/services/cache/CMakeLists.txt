add_executable(
    test_application_bundle_orm

    TestApplicationBundleORM.cpp

    ../../../src/services/Application.cpp
    ../../../src/services/ApplicationData.cpp
    ../../../src/services/ApplicationBundle.cpp

    ../../../src/services/cache/ApplicationBundleORM.cpp
)

target_link_libraries(test_application_bundle_orm PRIVATE Qt5::Widgets Qt5::Test Qt5::Sql)
target_include_directories(test_application_bundle_orm PRIVATE ../../../src)
add_test(NAME testApplicationBundleORM COMMAND test_application_bundle_orm)

add_executable(
    test_cache_service

    TestCacheService.cpp

    ../../../src/services/Application.cpp
    ../../../src/services/ApplicationData.cpp
    ../../../src/services/ApplicationBundle.cpp


    ../../../src/services/cache/ApplicationORM.cpp
    ../../../src/services/cache/ApplicationBundleORM.cpp

    ../../../src/services/cache/CacheService.cpp
)

target_link_libraries(test_cache_service PRIVATE Qt5::Widgets Qt5::Test Qt5::Sql)
target_include_directories(test_cache_service PRIVATE ../../../src)
add_test(NAME testCacheService COMMAND test_cache_service)