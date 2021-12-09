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

add_test(NAME testApplicationsRegistry COMMAND test_applications_registry)

add_executable(
    test_applications_data
    ../src/services/ApplicationData.cpp
    ../src/services/ApplicationBundle.cpp

    TestApplicationData.cpp
)
target_include_directories(test_applications_data PRIVATE ../src/services/)
target_link_libraries(test_applications_data PRIVATE Qt5::Widgets Qt5::Test)

add_test(NAME testApplicationsData COMMAND test_applications_data)


add_executable(
    test_applications_dirs_watcher

    ../src/services/ApplicationsDirsWatcher.cpp
    TestApplicationsDirsWatcher.cpp
)
target_include_directories(test_applications_dirs_watcher PRIVATE ../src/services/)
target_link_libraries(test_applications_dirs_watcher PRIVATE Qt5::Widgets Qt5::Test)

add_test(NAME testApplicationsDirsWatcher COMMAND test_applications_dirs_watcher)

set(APPIMAGE_TOOL_PATH ${CMAKE_CURRENT_BINARY_DIR}/data/appimagetool-x86_64.AppImage)
if (NOT EXISTS ${APPIMAGE_TOOL_PATH})
    set(APPIMAGE_TOOL_URL https://github.com/AppImage/AppImageKit/releases/download/13/appimagetool-x86_64.AppImage)
    file(DOWNLOAD ${APPIMAGE_TOOL_URL} ${APPIMAGE_TOOL_PATH})
endif (NOT EXISTS ${APPIMAGE_TOOL_PATH})

add_executable(
    test_bundle_inspector

    ../src/services/BundleInspector.cpp
    ../src/services/ApplicationData.cpp
    ../src/services/ApplicationBundle.cpp
    TestBundleInspector.cpp
)

kde_source_files_enable_exceptions(test_bundle_inspector ../src/services/BundleInspector.cpp)

target_include_directories(test_bundle_inspector PRIVATE ../src/services/)
target_link_libraries(test_bundle_inspector PRIVATE Qt5::Widgets Qt5::Test libappimage)
target_compile_definitions(test_bundle_inspector PRIVATE SAMPLE_APPIMAGE_PATH="${APPIMAGE_TOOL_PATH}")

add_test(NAME testBundleInspector COMMAND test_bundle_inspector)