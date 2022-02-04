enable_testing(true)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Test REQUIRED)

set(APPIMAGE_TOOL_PATH ${CMAKE_CURRENT_BINARY_DIR}/data/appimagetool-x86_64.AppImage)
if (NOT EXISTS ${APPIMAGE_TOOL_PATH})
    set(APPIMAGE_TOOL_URL https://github.com/AppImage/AppImageKit/releases/download/13/appimagetool-x86_64.AppImage)
    file(DOWNLOAD ${APPIMAGE_TOOL_URL} ${APPIMAGE_TOOL_PATH})
endif (NOT EXISTS ${APPIMAGE_TOOL_PATH})


add_executable(
    test_applications_registry
    ../src/services/Application.cpp
    ../src/services/ApplicationData.cpp
    ../src/services/ApplicationBundle.cpp
    ../src/services/registry/ApplicationsRegistry.cpp
    ../src/utils/appimagetools.cpp

    services/registry/TestApplicationsRegistry.cpp
)

kde_source_files_enable_exceptions(test_applications_registry ../src/utils/appimagetools.cpp)
target_include_directories(test_applications_registry PRIVATE ../src/)
target_link_libraries(test_applications_registry PRIVATE MauiKit MauiKit::FileBrowsing Qt5::Sql Qt5::Widgets Qt5::Test libappimage)

add_test(NAME testApplicationsRegistry COMMAND test_applications_registry)

add_executable(
    test_application
    ../src/services/Application.cpp
    ../src/services/ApplicationData.cpp
    ../src/services/ApplicationBundle.cpp

    TestApplication.cpp
)
target_include_directories(test_application PRIVATE ../src/)
target_link_libraries(test_application PRIVATE Qt5::Widgets Qt5::Test)

add_test(NAME testApplication COMMAND test_application)


add_executable(
    test_bundles_dirs_watcher

    ../src/services/registry/BundlesDirsWatcher.cpp
    ../src/services/registry/BundleInspector.cpp
    ../src/services/Application.cpp
    ../src/services/ApplicationData.cpp
    ../src/services/ApplicationBundle.cpp
    services/registry/TestBundlesDirsWatcher.cpp
)

kde_source_files_enable_exceptions(test_bundles_dirs_watcher ../src/services/registry/BundlesDirsWatcher.cpp)
target_include_directories(test_bundles_dirs_watcher PRIVATE ../src/services/)
target_link_libraries(test_bundles_dirs_watcher PRIVATE Qt5::Widgets Qt5::Test libappimage)
target_compile_definitions(test_bundles_dirs_watcher PRIVATE SAMPLE_APPIMAGE_PATH="${APPIMAGE_TOOL_PATH}")

add_test(NAME testBundlesDirsWatcher COMMAND test_bundles_dirs_watcher)

add_executable(
    test_bundle_inspector

    ../src/services/registry/BundleInspector.cpp
    ../src/services/Application.cpp
    ../src/services/ApplicationData.cpp
    ../src/services/ApplicationBundle.cpp
    services/registry/TestBundleInspector.cpp
)

kde_source_files_enable_exceptions(test_bundle_inspector ../src/services/registry/BundleInspector.cpp)

target_include_directories(test_bundle_inspector PRIVATE ../src/services/)
target_link_libraries(test_bundle_inspector PRIVATE Qt5::Widgets Qt5::Test libappimage)
target_compile_definitions(test_bundle_inspector PRIVATE SAMPLE_APPIMAGE_PATH="${APPIMAGE_TOOL_PATH}")

add_test(NAME testBundleInspector COMMAND test_bundle_inspector)

add_executable(
    test_update_service

    ../src/services/update/UpdateService.cpp
    ../src/services/update/ApplicationUpdateData.cpp
    ../src/services/update/UpdateWorker.cpp
    ../src/services/update/UpdateCheckWorker.cpp
    ../src/services/TaskData.cpp
    ../src/services/Application.cpp
    ../src/services/ApplicationData.cpp
    ../src/services/ApplicationBundle.cpp

    TestUpdateService.cpp
)

target_include_directories(test_update_service PRIVATE ../src/)
target_link_libraries(test_update_service PRIVATE Qt5::Widgets Qt5::Test libappimage AppImageUpdate::libappimageupdate)
target_compile_definitions(test_update_service PRIVATE SAMPLE_APPIMAGE_PATH="${APPIMAGE_TOOL_PATH}")

add_test(NAME testUpdateService COMMAND test_update_service)

add_subdirectory(services/cache)