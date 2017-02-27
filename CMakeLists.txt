project(nx_software_center)
cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0037 OLD)


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

find_package(Qt5 REQUIRED COMPONENTS Qml Quick Gui Widgets Network)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Auth)
find_package(Snapd REQUIRED)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

# add the qml.qrc file
qt5_add_resources(qml_QRC src/qml/files.qrc)

#qt5_add_resources(qml_QRC data/resources.qrc)


kauth_install_actions(org.nomad.softwarecenter org.nomad.softwarecenter.actions)

# add source files
set(SOURCE
        ${SOURCE}
        src/main.cpp
        src/snapdclientkauthwrapper.cpp
        src/snapdsettings.cpp
        src/snapstore/snapstore.cpp
        src/snapstore/snapstorerequest.cpp
        src/snapstore/snapstorelistdepartamentsrequest.cpp
        src/snapstore/snapstoregetdepartamentrequest.cpp
        src/snapstore/snapstoresnapdetailsrequest.cpp
        src/snapstore/customnetworkaccessmanagerfactory.cpp
)

set(HELPER_SOURCE
        src/helper/snapdhelper.cpp
)


message(NOTICE ${SNAPD_LIBRARIES})
add_executable(${PROJECT_NAME} ${SOURCE} ${qml_QRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${SNAPD_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Qt5::Qml Qt5::Widgets Qt5::Quick KF5::Auth ${SNAPD_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_executable(${PROJECT_NAME}_helper  ${HELPER_SOURCE})
target_include_directories(${PROJECT_NAME}_helper PUBLIC ${SNAPD_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_helper Qt5::Core Qt5::Network KF5::Auth ${SNAPD_LIBRARIES})

install(TARGETS ${PROJECT_NAME}_helper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(${PROJECT_NAME}_helper org.nomad.softwarecenter root)

install(DIRECTORY data/departaments_background/ DESTINATION share/${PROJECT_NAME}/departaments_background FILES_MATCHING PATTERN "*.png")
