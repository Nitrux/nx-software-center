enable_testing()

include(cmake/fetch_gtest.cmake)

######
# Configure tests
######

add_subdirectory(core)

find_package(Qt5 REQUIRED COMPONENTS Core Network Test)

add_executable(tests
#    $<TARGET_OBJECTS:core>
    src/tests_main.cpp
#    src/TestFetchApplicationsInteractor.cpp
#    src/DummySource.h src/BrokenDummySource.h
#    src/TestAppImageHubSource.cpp
#    src/TestAppImageInstallLinksRegExParser.cpp
#    src/FakeDownloadManager.cpp
#    src/FakeDownloadManager.h
    )

message(${Qt5Network_INCLUDE_DIRS})
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
#    Qt5::Network
#    Qt5::Test
    )
#target_link_libraries(tests
#    ${gtest_LIBRARIES}
#    pthread # this must be last else it fails at linking
#    )
add_dependencies(tests gtest)