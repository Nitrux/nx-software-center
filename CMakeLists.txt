project(nomad_software_center)
cmake_minimum_required(VERSION 3.2)

cmake_policy(SET CMP0037 NEW)
cmake_policy(SET CMP0063 NEW)

# Enbable C++11 support
set(CMAKE_CXX_STANDARD 14)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Qml Quick Gui Widgets Network Concurrent)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Auth KIO)
find_package(AppImageKit REQUIRED)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

# add the qml.qrc file
qt5_add_resources(qml_QRC src/qml/files.qrc)

# add source files
set (CORE_SOURCE
    src/entities/app.cpp
    src/entities/release.cpp
    src/entities/change.cpp
    src/entities/repository.cpp
    src/entities/registry.cpp
    src/entities/simplefileregistry.cpp
    src/entities/downloadmanager.h
    src/entities/progresslistener.h

    src/gateways/kf5downloadmanager.cpp
    src/gateways/appimagehubrepository.cpp

    src/interactors/interactor.cpp
    src/interactors/searchapplicationsinteractor.cpp
    src/interactors/searchapplicationsinteractorlistener.h
    src/interactors/downloadappimagereleaseinteractor.cpp
    src/interactors/downloadappimagereleaseinteractorlistener.h
    src/interactors/removeappimagereleaseinteractor.cpp
    src/interactors/removeappimagereleaseinteractorlistener.h
    src/interactors/executeappimagereleaseinteractor.cpp
    src/interactors/executeappimagereleaseinteractorlistener.h
    src/interactors/fetchappimagehubinteractor.cpp
    src/interactors/deployappimagereleaseinteractor.cpp
    )

add_library(core OBJECT ${CORE_SOURCE})
target_compile_options(core PUBLIC -fPIC -fexceptions)
target_include_directories(core
    PRIVATE ${Qt5Core_INCLUDE_DIRS}
    PRIVATE ${Qt5Network_INCLUDE_DIRS}
    PRIVATE /usr/include/KF5/KIOCore
    PRIVATE /usr/include/KF5/KCoreAddons
    )

set(SOURCE
        ${SOURCE}
        src/main.cpp

        src/ui/searchviewcontroller.cpp
        src/ui/registrycontroller.cpp

        src/ui/taskcontroller.cpp
        src/ui/taskscontroller.cpp
        src/ui/taskdownloadappimagecontroller.cpp
        src/ui/taskremoveappimagereleasecontroller.cpp
        src/ui/taskexecuteappimagecontroller.cpp
        src/ui/taskfetchappscontroller.cpp
)


add_executable(${PROJECT_NAME} ${SOURCE} ${qml_QRC} $<TARGET_OBJECTS:core>)
target_compile_options(${PROJECT_NAME} PUBLIC -fexceptions)
target_link_libraries(${PROJECT_NAME}
    Qt5::Qml
    Qt5::Widgets
    Qt5::Quick
    Qt5::Concurrent
    KF5::Auth
    KF5::KIOCore
    libappimage
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

install(DIRECTORY data/departaments_background/ DESTINATION share/${PROJECT_NAME}/departaments_background FILES_MATCHING PATTERN "*.png")
install(FILES "org.nxos.softwarecenter.desktop" DESTINATION share/applications)

# enable_testing()
# add_subdirectory(src/test)
