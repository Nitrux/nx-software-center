enable_testing()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Test Network Xml XmlPatterns)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Auth KIO)


###
# TEST ENTITIES
###

set(ENTITIES_TESTS_SOURCE
    entities_tests.cpp
    dummyrelease.cpp
    dummyrepository.cpp
    dummychange.cpp
    dummydownloadchange.cpp
    dummyinstallchange.cpp
    dummyuninstallchange.cpp
    dummyremovechange.cpp

    ${ENTITIES_SOURCE}
    )



add_executable(entities_tests ${ENTITIES_TESTS_SOURCE} $<TARGET_OBJECTS:core>)
target_compile_options(entities_tests PUBLIC -fexceptions)
target_link_libraries(entities_tests
    Qt5::Test
    Qt5::Qml
    Qt5::Widgets
    Qt5::Quick
    Qt5::Concurrent
    KF5::Auth
    KF5::KIOCore
    ${APPIMAGE_LIBRARIES}
)
add_test(entities_tests entities_tests)

###
# TEST INTERACTORS
###
set(INTERACTORS_TESTS_SOURCE
    interactors_tests.cpp
    dummydownloadmanager.cpp
    dummyrelease.cpp
    dummyrepository.cpp
    dummychange.cpp
    dummydownloadchange.cpp
    dummyinstallchange.cpp
    dummyuninstallchange.cpp
    dummyremovechange.cpp

    ${ENTITIES_SOURCE}
    ${INTERACTORS_SOURCE}
    )



add_executable(interactors_tests ${INTERACTORS_TESTS_SOURCE} $<TARGET_OBJECTS:core>)
target_compile_options(interactors_tests PUBLIC -fexceptions)
target_link_libraries(interactors_tests
    Qt5::Test
    Qt5::Qml
    Qt5::Widgets
    Qt5::Quick
    Qt5::Concurrent
    KF5::Auth
    KF5::KIOCore
    ${APPIMAGE_LIBRARIES}
)

add_test(interactors_tests interactors_tests)

###
# TEST GATEWAYS
###

set(GATEWAYS_TESTS_SOURCE
    gateways_tests.cpp
    dummydownloadmanager.cpp
    dummyrelease.cpp
    dummyrepository.cpp
    dummychange.cpp
    dummydownloadchange.cpp
    dummyinstallchange.cpp
    dummyuninstallchange.cpp
    dummyremovechange.cpp

    ${ENTITIES_SOURCE}
    ${GATEWAYS_SOURCE}
    )



add_executable(gateways_tests ${GATEWAYS_TESTS_SOURCE} $<TARGET_OBJECTS:core>)
target_compile_options(gateways_tests PUBLIC -fexceptions)
target_link_libraries(gateways_tests
    Qt5::Test
    Qt5::Qml
    Qt5::Widgets
    Qt5::Quick
    Qt5::Concurrent
    KF5::Auth
    KF5::KIOCore
    ${APPIMAGE_LIBRARIES}
)
add_test(gateways_tests gateways_tests)
