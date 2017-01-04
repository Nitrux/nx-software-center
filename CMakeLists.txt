project(nx_software_center)
cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0037 OLD)


# Enbable C++11 support
set(CMAKE_CXX_STANDARD 14)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the Qt libraries for Qt Quick/QML
find_package(Qt5Core CONFIG REQUIRED Qml Quick Gui)
find_package(Qt5Quick)
find_package(Qt5Qml)
find_package(Qt5Gui)
find_package(Qt5Network)

find_package(KDE4 REQUIRED)
find_package(KF5Auth NO_MODULE)

# add the qml.qrc file
qt5_add_resources(qml_QRC src/qml.qrc)

kde4_install_auth_actions(org.nomad.softwarecenter org.nomad.softwarecenter.actions)

# add source files
set(SOURCE
        ${SOURCE}
        src/main.cpp
        src/snapd.cpp
        src/httputils.cpp
        src/snapsindex.cpp
)

set(HELPER_SOURCE
        src/snapdhelper.cpp
        src/httputils.cpp
)


kde4_add_executable(${PROJECT_NAME} ${SOURCE} ${qml_QRC})
target_link_libraries(${PROJECT_NAME} Qt5::Qml Qt5::Quick KF5::Auth)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

kde4_add_executable(${PROJECT_NAME}_helper  ${HELPER_SOURCE})
target_link_libraries(${PROJECT_NAME}_helper Qt5::Core Qt5::Network KF5::Auth)

install(TARGETS ${PROJECT_NAME}_helper DESTINATION ${LIBEXEC_INSTALL_DIR})
kde4_install_auth_helper_files(${PROJECT_NAME}_helper org.nomad.softwarecenter root)


