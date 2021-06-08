# Copyright 2018-2020 Camilo Higuita <milo.h@aol.com>
# Copyright 2018-2020 Nitrux Latinoamericana S.C.
#
# SPDX-License-Identifier: GPL-3.0-or-later

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/stores
    ${CMAKE_CURRENT_BINARY_DIR}/src/stores

    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_BINARY_DIR}/utils
    )

set(nx_SRCS
    main.cpp
    models/appsmodel.cpp
    models/storemodel.cpp
    models/categoriesmodel.cpp
    models/app.cpp
    models/progressmanager.cpp
    )

set(nx_HDRS
    utils/nx.h
    models/appsmodel.h
    models/storemodel.h
    models/categoriesmodel.h
    models/app.h
    models/progressmanager.h
    )

set(nx_RSRCS
    assets/nx-assets.qrc
    qml.qrc
    )

set(nx_STORES_SRCS
    stores/opendesktopstore.cpp
    stores/appimagehubstore.cpp
    stores/ResponseDTO/category.cpp
    stores/ResponseDTO/application.cpp
    )


set(nx_STORES_HDRS
    stores/store.h
    stores/opendesktopstore.h
    stores/appimagehubstore.h

    stores/ResponseDTO/category.h
    stores/ResponseDTO/categoryresponsedto.h
    stores/ResponseDTO/application.h
    stores/ResponseDTO/applicationresponsedto.h)

if(ANDROID)
    add_library(${PROJECT_NAME} SHARED
        ${nx_SRCS}
        ${nx_HDRS}
        ${nx_RSRCS}

        ${nx_STORES_SRCS}
        ${nx_STORES_HDRS})
else()
    add_executable(${PROJECT_NAME}
        ${nx_SRCS}
        ${nx_HDRS}
        ${nx_RSRCS}

        ${nx_STORES_SRCS}
        ${nx_STORES_HDRS})
endif()


if (ANDROID)
    target_link_libraries(${PROJECT_NAME} Qt5::AndroidExtras)
#     kde_source_files_enable_exceptions(pix src/pix.cpp)
else()
    target_link_libraries(${PROJECT_NAME} KF5::ConfigCore KF5::Notifications KF5::KIOCore KF5::I18n KF5::KIOWidgets KF5::KIOFileWidgets)
endif()

target_link_libraries(${PROJECT_NAME} MauiKit MauiKit::FileBrowsing Qt5::Qml Qt5::Svg Qt5::WebEngine Qt5::Sql)

if(UNIX AND NOT APPLE AND NOT ANDROID)
    install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
    install(FILES org.nx.softwarecenter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
    install(FILES assets/nx-software-center.svg DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps)
endif()
