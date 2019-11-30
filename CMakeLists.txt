project(nx-software-center)
cmake_minimum_required(VERSION 3.0)

find_package(ECM 1.7.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH})

find_package(MauiKit REQUIRED)
find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Qml Quick Svg QuickControls2 Widgets WebEngine Sql)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(FeatureSummary)
include(ECMAddAppIcon)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stores
    ${CMAKE_CURRENT_BINARY_DIR}/src/stores
    )

set(nx_SRCS
    src/main.cpp
    src/models/appsmodel.cpp
    src/models/storemodel.cpp
    src/models/categoriesmodel.cpp
    )

set(nx_HDRS
    src/utils/nx.h
    src/models/appsmodel.h
    src/models/storemodel.h
    src/models/categoriesmodel.h
    )

set(nx_RSRCS
    assets/nx-assets.qrc
    src/qml.qrc
    )

set(nx_STORES_SRCS
    src/stores/opendesktopstore.cpp
    src/stores/appimagehubstore.cpp
    src/stores/ResponseDTO/category.cpp)

set(nx_STORES_HDRS
    src/stores/store.h
    src/stores/opendesktopstore.h
    src/stores/appimagehubstore.h

    src/stores/ResponseDTO/category.h
    src/stores/ResponseDTO/categoryresponsedto.h
    src/stores/ResponseDTO/application.h
    src/stores/ResponseDTO/applicationresponsedto.h)

add_executable(${PROJECT_NAME}
    ${nx_SRCS}
    ${nx_HDRS}
    ${nx_RSRCS}

    ${nx_STORES_SRCS}
    ${nx_STORES_HDRS})

if (ANDROID)
    find_package(Qt5 REQUIRED COMPONENTS AndroidExtras)

    target_link_libraries(vvave Qt5::AndroidExtras)
#     kde_source_files_enable_exceptions(pix src/pix.cpp)
else()
    find_package(KF5 ${KF5_VERSION} REQUIRED COMPONENTS I18n Notifications Config KIO Attica SyntaxHighlighting)
    target_link_libraries(${PROJECT_NAME} KF5::ConfigCore KF5::Notifications KF5::KIOCore KF5::I18n KF5::KIOWidgets KF5::KIOFileWidgets)
endif()

target_link_libraries(${PROJECT_NAME} MauiKit Qt5::Qml Qt5::Widgets Qt5::Svg Qt5::WebEngine Qt5::Sql)

install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES org.nx.softwarecenter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

install(FILES assets/nx-software-center.svg DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
