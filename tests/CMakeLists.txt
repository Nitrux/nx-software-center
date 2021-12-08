enable_testing(true)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Test REQUIRED)


add_executable(
    test_applications_registry
    ../src/services/ApplicationData.cpp
    ../src/services/ApplicationBundle.cpp
    ../src/services/ApplicationsRegistry.cpp

    TestApplicationsRegistry.cpp
)
target_include_directories(test_applications_registry PRIVATE ../src/services/)
target_link_libraries(test_applications_registry PRIVATE Qt5::Widgets Qt5::Test)

add_test(NAME testAppsRegistry COMMAND test_applications_registry)

add_executable(
    test_applications_data
    ../src/services/ApplicationData.cpp
    ../src/services/ApplicationBundle.cpp

    TestApplicationData.cpp
)
target_include_directories(test_applications_data PRIVATE ../src/services/)
target_link_libraries(test_applications_data PRIVATE Qt5::Widgets Qt5::Test)

add_test(NAME testApplicationsData COMMAND test_applications_data)
