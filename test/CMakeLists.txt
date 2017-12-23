enable_testing()

include(cmake/fetch_gtest.cmake)

######
# Configure tests
######

add_subdirectory(core)

find_package(Qt5 REQUIRED COMPONENTS Test Core Network)

add_executable(tests
    $<TARGET_OBJECTS:core>
    src/tests_main.cpp
    src/TestFetchApplicationsInteractor.cpp
    src/DummySource.h src/BrokenDummySource.h
    src/TestAppImageHubSource.cpp
    src/TestAppImageInstallLinksRegExParser.cpp src/FakeDownloadManager.cpp src/FakeDownloadManager.h)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src/)

target_compile_definitions(tests
    PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/"
    )

target_link_libraries(tests
    Qt5::Core
    Qt5::Network
    Qt5::Test

    ${gtest_LIBRARIES}
    pthread # this must be last else it fails at linking
    )

add_dependencies(tests gtest)