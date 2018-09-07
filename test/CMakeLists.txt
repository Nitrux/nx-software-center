enable_testing()

include(cmake/fetch_gtest.cmake)

######
# Configure tests
######

find_package(Qt5 REQUIRED COMPONENTS Core Gui Network Test XmlPatterns)

add_executable(test_core
        $<TARGET_OBJECTS:core>
        src/test_core_main.cpp
        src/TestDownload.cpp
        src/TestTaskLogger.cpp
        src/TestUpgrader.cpp
        src/TestApplicationRepositoryRestClient.cpp
        src/TestAppImageInfo.cpp
        src/TestInstaller.cpp
        src/DummyApplicationRepository.cpp
        src/DummyApplicationRepository.h
        src/DummyApplicationRepositorySearch.hpp
        src/DummyApplicationRepositoryGet.hpp
        src/TestDeployedApplicationsRegistry.cpp)

target_include_directories(test_core
        PRIVATE ${CMAKE_SOURCE_DIR}/src/
        PRIVATE ${gtest_INCLUDE_DIRS}
        PRIVATE ${Qt5Core_INCLUDE_DIRS}
        PRIVATE ${Qt5Network_INCLUDE_DIRS}
        PRIVATE ${Qt5XmlPatterns_INCLUDE_DIRS}
        )

target_compile_definitions(test_core
        PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/"
        )

target_link_libraries(test_core
        Qt5::Core
        Qt5::Gui
        Qt5::Network
        Qt5::Test
        Qt5::XmlPatterns

        libappimage
        libappimageinfo

        ${gtest_LIBRARIES}
        pthread # this must be last else it fails at linking
        )

add_dependencies(test_core gtest)

set_target_properties(test_core PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_test(software_center_core test_core)
