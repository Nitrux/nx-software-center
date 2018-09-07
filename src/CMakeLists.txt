find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Network XmlPatterns)

###
# Core
###

set(CORE_SRC
        entities/Application.cpp
        entities/Application.h
        entities/ApplicationAbstract.cpp
        entities/ApplicationAbstract.h

        gateways/ApplicationRepositoryRestClient.cpp
        gateways/ApplicationRepositoryRestClient.h

        entities/Deployer.cpp
        entities/Deployer.h
        entities/TaskLogger.cpp
        entities/TaskLogger.h
        gateways/FileDownload.h
        gateways/FileDownload.cpp
        tasks/TaskMetadata.h
        entities/Upgrader.cpp
        entities/Upgrader.h
        entities/AppImageInfo.cpp
        entities/AppImageInfo.h
        gateways/ApplicationsSearchRequest.cpp
        gateways/ApplicationsSearchRequest.h
        gateways/ApplicationGetRequest.cpp
        gateways/ApplicationGetRequest.h
        tasks/DeployTask.cpp
        tasks/DeployTask.h
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
        tasks/RemoveTask.cpp
        tasks/RemoveTask.h
        tasks/UpgradeTask.cpp
        tasks/UpgradeTask.h
        entities/LocalizationUtils.cpp
        entities/LocalizationUtils.h
        )

add_library(core OBJECT ${CORE_SRC})

get_target_property(LIBAPPIMAGE_INCLUDE_DIRS libappimage INTERFACE_INCLUDE_DIRECTORIES )
get_target_property(LIBAPPIMAGEINFO_INCLUDE_DIRS libappimageinfo INTERFACE_INCLUDE_DIRECTORIES )
target_compile_options(core PUBLIC -fPIC -fexceptions)
target_include_directories(core
        PUBLIC ${Qt5Core_INCLUDE_DIRS}
        PUBLIC ${Qt5Network_INCLUDE_DIRS}
        PUBLIC ${Qt5Gui_INCLUDE_DIRS}
        PUBLIC ${Qt5Qml_INCLUDE_DIRS}
        PUBLIC ${LIBAPPIMAGE_INCLUDE_DIRS}
        PUBLIC ${LIBAPPIMAGEINFO_INCLUDE_DIRS}
        PUBLIC ${NLOHMANN_JSON_INCLUDE_DIRS}
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
        ui/TaskLoggerController.h
        ui/TaskLoggerController.cpp
        ui/RegistryListModel.h
        ui/RegistryListModel.cpp
        ui/RemoveController.cpp
        ui/RemoveController.h
        ui/UpgraderController.cpp
        ui/UpgraderController.h
        ui/NotificationsController.cpp
        ui/NotificationsController.h
        ui/RunController.cpp
        ui/RunController.h
        ui/ApplicationViewController.cpp
        ui/ApplicationViewController.h
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
        $<TARGET_OBJECTS:gui>)

target_link_libraries(nx_software_center
        Qt5::Core
        Qt5::Gui
        Qt5::Qml
        Qt5::Network
        Qt5::XmlPatterns
        libappimage
        libappimageinfo)

install(TARGETS nx_software_center
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)

# Wrapper used to build the AppImage
add_executable(nx_software_center_wrapper wrapper.c )
target_compile_definitions(nx_software_center_wrapper
        PRIVATE BINARY_PATH="${CMAKE_INSTALL_PREFIX}/bin/nx_software_center")
install(TARGETS nx_software_center_wrapper
        RUNTIME DESTINATION bin)
