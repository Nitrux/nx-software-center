find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Network)
#find_package(AppImage REQUIRED)

set(CORE_SRC
    Application.cpp
    Repository.cpp
    Source.cpp
    Interactor.h
    DownloadManager.h

    AppImageHubSource.cpp
    FetchApplicationsInteractor.cpp
    AppImageInstallLinksRegExParser.cpp
    SimpleDownloadManager.cpp
    SimpleDownloadToMemoryJob.cpp
    SimpleDownloadToMemoryJob.h)

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

set (UI_SRC
    ApplicationListModel.cpp
    SearchControler.cpp)
## add the qml.qrc file
qt5_add_resources(qml_QRC qml/files.qrc)

add_executable(nx_software_center
    main.cpp
    FakeDownloadManager.cpp
    ${UI_SRC}
    ${qml_QRC})

target_sources(nx_software_center
    PRIVATE $<TARGET_OBJECTS:core>)

target_link_libraries(nx_software_center
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Network)

## add source files
#set (CORE_SOURCE
#    entities/app.cpp
#    entities/release.cpp
#    entities/change.cpp
#    entities/repository.cpp
#    entities/registry.cpp
#    entities/simplefileregistry.cpp
#    entities/downloadmanager.h
#    entities/progresslistener.h

#    gateways/kf5downloadmanager.cpp
#    gateways/appimagehubrepository.cpp

#    interactors/interactor.cpp
#    interactors/searchapplicationsinteractor.cpp
#    interactors/searchapplicationsinteractorlistener.h
#    interactors/downloadappimagereleaseinteractor.cpp
#    interactors/downloadappimagereleaseinteractorlistener.h
#    interactors/removeappimagereleaseinteractor.cpp
#    interactors/removeappimagereleaseinteractorlistener.h
#    interactors/executeappimagereleaseinteractor.cpp
#    interactors/executeappimagereleaseinteractorlistener.h
#    interactors/fetchappimagehubinteractor.cpp
#    interactors/deployappimagereleaseinteractor.cpp
#    )

#add_library(core OBJECT ${CORE_SOURCE})
#target_compile_options(core PUBLIC -fPIC -fexceptions)
#target_include_directories(core
#    PRIVATE ${Qt5Core_INCLUDE_DIRS}
#    PRIVATE ${Qt5Network_INCLUDE_DIRS}
#    PRIVATE /usr/include/KF5/KIOCore
#    PRIVATE /usr/include/KF5/KCoreAddons
#    )

#set(SOURCE
#        ${SOURCE}
#        main.cpp

#        ui/searchviewcontroller.cpp
#        ui/registrycontroller.cpp

#        ui/taskcontroller.cpp
#        ui/taskscontroller.cpp
#        ui/taskdownloadappimagecontroller.cpp
#        ui/taskremoveappimagereleasecontroller.cpp
#        ui/taskexecuteappimagecontroller.cpp
#        ui/taskfetchappscontroller.cpp
#)


#add_executable(${PROJECT_NAME} ${SOURCE} ${qml_QRC} $<TARGET_OBJECTS:core>)
#target_compile_options(${PROJECT_NAME} PUBLIC -fexceptions)
#target_link_libraries(${PROJECT_NAME}
#    Qt5::Qml
#    Qt5::Widgets
#    Qt5::Quick
#    Qt5::Concurrent
#    KF5::Auth
#    KF5::KIOCore
#    ${APPIMAGE_LIBRARIES}
#)

#install(TARGETS ${PROJECT_NAME} DESTINATION bin)

#install(DIRECTORY data/departaments_background/ DESTINATION share/${PROJECT_NAME}/departaments_background FILES_MATCHING PATTERN "*.png")
#install(FILES "org.nxos.softwarecenter.desktop" DESTINATION share/applications)
