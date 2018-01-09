find_package(Qt5 REQUIRED COMPONENTS Test Core)
#find_package(KF5 REQUIRED COMPONENTS CoreAddons )

add_executable(test_Application test_application.cpp $<TARGET_OBJECTS:core>)
target_include_directories(test_Application PRIVATE ${CMAKE_SOURCE_DIR}/src/ )

target_link_libraries(test_Application
    Qt5::Test
    Qt5::Core
)

add_test(test_Application test_Application)


add_executable(test_Repository test_repository.cpp $<TARGET_OBJECTS:core>)
target_include_directories(test_Repository PRIVATE ${CMAKE_SOURCE_DIR}/src/ )

target_link_libraries(test_Repository
    Qt5::Test
    Qt5::Core
    Qt5::Network
)

add_test(test_Repository test_Repository)

add_executable(test_Source $<TARGET_OBJECTS:core> test_source.cpp)
target_include_directories(test_Source PRIVATE ${CMAKE_SOURCE_DIR}/src/ )

target_link_libraries(test_Source
    Qt5::Test
    Qt5::Core
    )

add_test(test_Repository test_Repository)