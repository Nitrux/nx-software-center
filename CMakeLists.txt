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

find_package(Qt5 REQUIRED COMPONENTS Qml Quick Gui Network)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Auth)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

# add the qml.qrc file
qt5_add_resources(qml_QRC src/qml.qrc)

kauth_install_actions(org.nomad.softwarecenter org.nomad.softwarecenter.actions)

# add source files
set(SOURCE
        ${SOURCE}
        src/main.cpp
        src/snapd.cpp
        src/changes.cpp
        src/httputils.cpp
        src/snapsindex.cpp
        src/localsnaps.cpp
        src/snapstore.cpp
)

set(HELPER_SOURCE
        src/snapdhelper.cpp
        src/httputils.cpp
)


add_executable(${PROJECT_NAME} ${SOURCE} ${qml_QRC})
target_link_libraries(${PROJECT_NAME} Qt5::Qml Qt5::Quick KF5::Auth)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_executable(${PROJECT_NAME}_helper  ${HELPER_SOURCE})
target_link_libraries(${PROJECT_NAME}_helper Qt5::Core Qt5::Network KF5::Auth)

install(TARGETS ${PROJECT_NAME}_helper DESTINATION ${LIBEXEC_INSTALL_DIR})
kauth_install_helper_files(${PROJECT_NAME}_helper org.nomad.softwarecenter root)


