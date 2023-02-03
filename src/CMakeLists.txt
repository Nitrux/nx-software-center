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
    NXSCApp.cpp
    models/storemodel.cpp
    models/categoriesmodel.cpp
    models/app.cpp
    models/ApplicationsRegistryModel.cpp
    models/ApplicationsRegistryProxyModel.cpp
    models/TasksListModel.cpp
    services/storemanager/storemanagerservice.cpp
    services/storemanager/apprepostoremanager.cpp
    services/storemanager/appimagehubstoremanager.cpp
    utils/appimagetools.cpp
    services/registry/ApplicationsRegistry.cpp
    services/ApplicationData.cpp
    services/Application.cpp
    services/ApplicationBundle.cpp
    services/registry/BundlesDirsWatcher.cpp
    services/registry/BundleInspector.cpp
    services/update/UpdateService.cpp
    services/update/ApplicationUpdateData.cpp
    services/update/UpdateWorker.cpp
    services/update/UpdateCheckWorker.cpp
    services/TaskData.cpp
    services/install/InstallService.cpp
    services/install/InstallWorker.cpp
    services/launch/LaunchService.cpp
    services/delete/DeleteService.cpp
    services/cache/CacheService.cpp
    services/cache/ApplicationORM.cpp
    services/cache/ApplicationBundleORM.cpp
    server/server.cpp
    )

set(nx_HDRS
    utils/nx.h
    models/storemodel.h
    models/categoriesmodel.h
    models/app.h
    services/storemanager/storemanager.h
    utils/appimagetools.h
    )

set(nx_RSRCS
    assets/nx-assets.qrc
    data/data.qrc
    )

set(nx_STORES_SRCS
    stores/opendesktopstore.cpp
    stores/appimagehubstore.cpp
    stores/apprepostore.cpp
    stores/ResponseDTO/category.cpp
    stores/ResponseDTO/pling_store_application_data.cpp
    stores/ResponseDTO/apprepo/apprepogroupresponsedto.cpp
    stores/ResponseDTO/apprepo/apprepopackageresponsedto.cpp
    stores/ResponseDTO/apprepo/apprepoversiondto.cpp
    )

set(nx_STORES_HDRS
    stores/store.h
    stores/opendesktopstore.h
    stores/appimagehubstore.h
    stores/apprepostore.h

    stores/ResponseDTO/category.h
    stores/ResponseDTO/categoryresponsedto.h
    stores/ResponseDTO/pling_store_application_data.h
    stores/ResponseDTO/applicationresponsedto.h)

if (QUICK_COMPILER)
    qtquick_compiler_add_resources(nx_QML_QRC qml.qrc)
else ()
    qt5_add_resources(nx_QML_QRC qml.qrc)
endif ()

)

qt_generate_dbus_interface(server/server.h org.nx.SoftwareCenter.xml)
qt_add_dbus_adaptor(nx_dbus_SRCS ${CMAKE_CURRENT_BINARY_DIR}/org.nx.SoftwareCenter.xml server/server.h Server)
qt_add_dbus_interface(nx_dbus_SRCS ${CMAKE_CURRENT_BINARY_DIR}/org.nx.SoftwareCenter.xml serverinterface)

add_executable(${PROJECT_NAME}
    ${nx_SRCS}
    ${nx_HDRS}
    ${nx_RSRCS}
    ${nx_QML_QRC}
    ${nx_dbus_SRCS}
    ${nx_STORES_SRCS}
    ${nx_STORES_HDRS})

kde_source_files_enable_exceptions(${PROJECT_NAME} utils/appimagetools.cpp services/registry/BundleInspector.cpp services/registry/ApplicationsRegistry.cpp)

target_link_libraries(${PROJECT_NAME} MauiKit MauiKit::FileBrowsing Qt5::Widgets Qt5::Quick Qt5::Qml Qt5::Sql Qt5::DBus KF5::I18n KF5::CoreAddons libappimageupdate libappimage)

install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES org.nx.softwarecenter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES assets/nx-software-center.svg DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps)
install(FILES data/org.nx.softwarecenter.notifyrc DESTINATION ${KNOTIFYRC_INSTALL_DIR})
