find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Network)

set(CORE_SRC
    entities/Application.cpp
    entities/Repository.cpp
    entities/Source.cpp
    interactors/Interactor.h
    gateways/DownloadManager.h

    gateways/AppImageHubSource.cpp
    interactors/FetchApplicationsInteractor.cpp
    gateways/AppImageInstallLinksRegExParser.cpp
    gateways/SimpleDownloadManager.cpp
    entities/Registry.cpp
    entities/Registry.h
    entities/Executor.cpp
    entities/Executor.h
    interactors/InstallAppImageInteractor.cpp
    interactors/InstallAppImageInteractor.h
    gateways/FileDownload.cpp
    gateways/FileDownload.h
    gateways/Download.h
    gateways/Download.cpp
    gateways/ContentDownload.cpp
    gateways/ContentDownload.h
    gateways/CachedDownloadManager.h
    gateways/CachedDownloadManager.cpp
    gateways/CacheSource.cpp
    gateways/CacheSource.h
    interactors/TaskMetadata.h
    entities/RecordMetadata.h
    interactors/RemoveAppImageInteractor.cpp
    interactors/RemoveAppImageInteractor.h
    entities/Upgrader.cpp
    entities/Upgrader.h
    entities/Updater.cpp
    entities/Updater.h
    entities/Cache.cpp
    entities/Cache.h
    entities/ApplicationSerializer.cpp
    entities/ApplicationSerializer.h
    )

add_library(core OBJECT ${CORE_SRC})

target_compile_options(core PUBLIC -fPIC -fexceptions)
target_include_directories(core
    PUBLIC ${Qt5Core_INCLUDE_DIRS}
    PUBLIC ${Qt5Network_INCLUDE_DIRS}
    PUBLIC ${Qt5Gui_INCLUDE_DIRS}
    PUBLIC ${Qt5Qml_INCLUDE_DIRS}
    )


######
# Main
######

set(UI_SRC
    ui/ApplicationListModel.h
    ui/ApplicationListModel.cpp
    ui/SearchControler.cpp
    ui/TasksController.cpp
    ui/TasksController.h
    ui/TaskListModel.cpp
    ui/TaskListModel.h
    ui/InstallController.h
    ui/InstallController.cpp
    ui/RegistryController.h
    ui/RegistryController.cpp
    ui/RegistryListModel.h
    ui/RegistryListModel.cpp
    ui/UninstallController.cpp
    ui/UninstallController.h
    ui/UpdaterController.cpp
    ui/UpdaterController.h
    ui/UpgraderController.cpp
    ui/UpgraderController.h
    ui/UpgradesListModel.h
    ui/UpgradesListModel.cpp)

## add the qml.qrc file
qt5_add_resources(qml_QRC qml/files.qrc)

add_executable(nx_software_center
    main.cpp
    ${UI_SRC}
    ${qml_QRC})

target_sources(nx_software_center
    PRIVATE $<TARGET_OBJECTS:core>)

target_link_libraries(nx_software_center
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Network)

install(FILES "org.nxos.softwarecenter.desktop" DESTINATION share/applications)
