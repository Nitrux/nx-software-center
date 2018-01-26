enable_testing()

include(cmake/fetch_gtest.cmake)

######
# Configure tests
######

#add_subdirectory(core)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Network Test)

add_executable(tests
    $<TARGET_OBJECTS:core>
    src/tests_main.cpp
    src/TestFetchApplicationsInteractor.cpp
    src/DummySource.h src/BrokenDummySource.h
    src/TestAppImageHubSource.cpp
    src/TestAppImageInstallLinksRegExParser.cpp
    src/TestExecutor.cpp
    src/TestDownload.cpp
    src/FakeDownloadManager.h src/FakeDownloadManager.cpp
    src/TestRegistry.cpp
    src/TestUpdater.cpp
    src/TestCache.cpp
    src/TestApplicationSerializer.cpp src/TestUpgrader.cpp)

target_include_directories(tests
    PRIVATE ${CMAKE_SOURCE_DIR}/src/
    PRIVATE ${Qt5Core_INCLUDE_DIRS}
    PRIVATE ${Qt5Network_INCLUDE_DIRS}
    )

target_compile_definitions(tests
    PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/"
    )

target_link_libraries(tests
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Test

    ${gtest_LIBRARIES}
    pthread # this must be last else it fails at linking
    )

add_test(software_center_core tests)