find_package(Qt5 REQUIRED COMPONENTS Core Network)
#find_package(KF5 REQUIRED COMPONENTS CoreAddons Auth KIO)
#find_package(AppImage REQUIRED)

add_library(core OBJECT
    Application.cpp
    Repository.cpp
    Source.cpp
    Interactor.h
    DownloadManager.h

    AppImageHubSource.cpp
    FetchApplicationsInteractor.cpp
    AppImageInstallLinksRegExParser.cpp
    SimpleDownloadManager.cpp SimpleDownloadToMemoryJob.cpp SimpleDownloadToMemoryJob.h)

target_compile_options(core PUBLIC -fPIC -fexceptions)
target_include_directories(core
    PUBLIC ${Qt5Core_INCLUDE_DIRS}
    PUBLIC ${Qt5Network_INCLUDE_DIRS})

## add the qml.qrc file
#qt5_add_resources(qml_QRC src/qml/files.qrc)

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
