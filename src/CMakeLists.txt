find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Network XmlPatterns)

###
# Core
###

set(CORE_SRC
        entities/ApplicationFull.cpp
        entities/ApplicationFull.h
        entities/Application.cpp
        entities/ApplicationAbstract.cpp
        entities/ApplicationAbstract.h

        entities/Repository.cpp
        entities/Source.cpp
        gateways/ApplicationRepositoryRestClient.cpp
        gateways/ApplicationRepositoryRestClient.h
        interactors/Interactor.h
        gateways/DownloadManager.h

        entities/Deployer.cpp
        entities/Deployer.h
        interactors/FetchApplicationsInteractor.cpp
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
        gateways/CacheSource.cpp
        gateways/CacheSource.h
        interactors/TaskMetadata.h
        entities/RecordMetadata.h
        interactors/RemoveAppImageInteractor.cpp
        interactors/RemoveAppImageInteractor.h
        entities/Upgrader.cpp
        entities/Upgrader.h
        entities/Cache.cpp
        entities/Cache.h
        entities/ApplicationSerializer.cpp
        entities/ApplicationSerializer.h
        interactors/RunInteractor.cpp
        interactors/RunInteractor.h
        entities/AppImageInfo.cpp
        entities/AppImageInfo.h
        gateways/ApplicationsSearchRequest.cpp
        gateways/ApplicationsSearchRequest.h
        gateways/ApplicationGetRequest.cpp
        gateways/ApplicationGetRequest.h
        entities/DeployTask.cpp
        entities/DeployTask.h
        entities/Task.cpp
        entities/Task.h
        entities/Worker.cpp
        entities/Worker.h
        gateways/ApplicationsRepositorySearch.cpp
        gateways/ApplicationsRepositorySearch.h
        gateways/ApplicationRepositoryGet.cpp
        gateways/ApplicationRepositoryGet.h
        gateways/ApplicationRepository.cpp
        gateways/ApplicationRepository.h
        gateways/DeployedApplicationsRegistry.cpp
        gateways/DeployedApplicationsRegistry.h
        entities/DeployedApplicationAbstract.cpp
        entities/DeployedApplicationAbstract.h
        entities/Remover.cpp
        entities/Remover.h
        entities/RemoveTask.cpp
        entities/RemoveTask.h)

add_library(core OBJECT ${CORE_SRC})

target_compile_options(core PUBLIC -fPIC -fexceptions)
target_include_directories(core
        PUBLIC ${Qt5Core_INCLUDE_DIRS}
        PUBLIC ${Qt5Network_INCLUDE_DIRS}
        PUBLIC ${Qt5Gui_INCLUDE_DIRS}
        PUBLIC ${Qt5Qml_INCLUDE_DIRS}
        PUBLIC ${libappimage_INCLUDE_DIRECTORIES}
        PUBLIC ${APPIMAGE_INFO_INCLUDE_DIRECTORIES}
        )

###
# GUI
###
set(CMAKE_AUTORCC ON)

set(GUI_SRC
        ui/ApplicationListModel.h
        ui/ApplicationListModel.cpp
        ui/SearchController.cpp
        ui/TasksController.cpp
        ui/TasksController.h
        ui/TaskListModel.cpp
        ui/TaskListModel.h
        ui/DeployController.h
        ui/DeployController.cpp
        ui/RegistryController.h
        ui/RegistryController.cpp
        ui/RegistryListModel.h
        ui/RegistryListModel.cpp
        ui/RemoveController.cpp
        ui/RemoveController.h
        ui/UpgraderController.cpp
        ui/UpgraderController.h
        ui/UpgradesListModel.h
        ui/UpgradesListModel.cpp
        ui/NotificationsController.cpp
        ui/NotificationsController.h
        ui/RunController.cpp
        ui/RunController.h
        ui/ApplicationViewController.cpp
        ui/ApplicationViewController.h
        ui/LocalizationUtils.cpp
        ui/LocalizationUtils.h
        ui/DeployedApplicationsController.cpp
        ui/DeployedApplicationsController.h
        )

## add the qml.qrc file
qt5_add_resources(qml_QRC qml/files.qrc)
qt5_add_resources(qml_QRC ../res/res.qrc)

add_library(gui OBJECT ${GUI_SRC} ${qml_QRC})

target_compile_options(gui PUBLIC -fPIC -fexceptions)
target_include_directories(gui
        PUBLIC ${Qt5Core_INCLUDE_DIRS}
        PUBLIC ${Qt5Gui_INCLUDE_DIRS}
        PUBLIC ${Qt5Qml_INCLUDE_DIRS}
        PUBLIC ${Qt5XmlPatterns_INCLUDE_DIRS}
        )


######
# Main
######

add_executable(nx_software_center
        main.cpp
        $<TARGET_OBJECTS:core>
        $<TARGET_OBJECTS:gui>
        $<TARGET_OBJECTS:appimage-info>)

target_link_libraries(nx_software_center
        Qt5::Core
        Qt5::Gui
        Qt5::Qml
        Qt5::Network
        Qt5::XmlPatterns

        libappimage)

install(TARGETS nx_software_center ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

