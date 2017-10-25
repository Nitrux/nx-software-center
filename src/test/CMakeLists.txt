enable_testing()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Test Network Xml XmlPatterns)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Auth KIO)


set(APPIMAGE_TEST_SOURCES
    appimage_test.cpp
    ../appimage/appimage.cpp
    ../appimage/appimagedownloadjob.cpp
    ../appimage/appimageuninstalljob.cpp
    ../appimage/appimagerepository.cpp
    ../appimage/appimagehubrepository.cpp
    )

add_executable(app_image_tests ${APPIMAGE_TEST_SOURCES})
add_test(app_image_tests app_image_tests)

target_link_libraries(app_image_tests Qt5::Test Qt5::Network KF5::CoreAddons Qt5::Xml Qt5::XmlPatterns KF5::KIOCore)


###
# TEST ENTITIES
###
set(ENTITIES_TESTS_SOURCE
    entities_tests.cpp
    dummyrelease.cpp
    dummychange.cpp
    dummyrepository.cpp
    ../entities/app.cpp
    ../entities/release.cpp
    ../entities/change.cpp
    ../entities/repository.cpp
    ../entities/registry.cpp
    ../entities/system.cpp
    )



add_executable(entities_tests ${ENTITIES_TESTS_SOURCE})
target_link_libraries(entities_tests Qt5::Test)
add_test(entities_tests entities_tests)


