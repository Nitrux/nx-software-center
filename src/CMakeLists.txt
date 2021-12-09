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
    models/package.cpp
    models/progressmanager.cpp
    utils/appimagetools.cpp
    services/ApplicationsRegistry.cpp
    services/ApplicationData.cpp
    services/ApplicationBundle.cpp
    services/ApplicationsDirsWatcher.cpp
    services/TaskManager.cpp
    services/Task.cpp
    services/TaskAction.cpp
    services/UpdateTask.cpp
    services/DownloadTask.cpp
    services/CheckUpdateTask.cpp
    services/CheckUpdateBulkTask.cpp
    services/UpdateAllTask.cpp
    )

set(nx_HDRS
    utils/nx.h
    models/appsmodel.h
    models/storemodel.h
    models/categoriesmodel.h
    models/app.h
    models/package.h
    models/progressmanager.h
    utils/appimagetools.h
    )

set(nx_RSRCS
    assets/nx-assets.qrc
    data/data.qrc
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

if(QUICK_COMPILER)
        qtquick_compiler_add_resources(nx_QML_QRC qml.qrc)
else()
        qt5_add_resources(nx_QML_QRC qml.qrc)
endif()

add_executable(${PROJECT_NAME}
    ${nx_SRCS}
    ${nx_HDRS}
    ${nx_RSRCS}
    ${nx_QML_QRC}

    ${nx_STORES_SRCS}
    ${nx_STORES_HDRS})

kde_source_files_enable_exceptions(${PROJECT_NAME} utils/appimagetools.cpp)

target_link_libraries(${PROJECT_NAME} MauiKit MauiKit::FileBrowsing Qt5::Widgets Qt5::Qml KF5::I18n KF5::CoreAddons AppImageUpdate::libappimageupdate libappimage)

install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES org.nx.softwarecenter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES assets/nx-software-center.svg DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps)
install(FILES data/org.nx.softwarecenter.notifyrc DESTINATION ${KNOTIFYRC_INSTALL_DIR})
