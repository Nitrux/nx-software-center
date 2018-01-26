project(nomad_software_center)
cmake_minimum_required(VERSION 3.2)

cmake_policy(SET CMP0037 NEW)
cmake_policy(SET CMP0063 NEW)

# Enable C++14 support
set(CMAKE_CXX_STANDARD 14)

# Enable exceptions
add_compile_options("-fexceptions")

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
include(ExternalProject)


add_subdirectory(src)
add_subdirectory(test)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

install(FILES "org.nxos.softwarecenter.desktop" DESTINATION share/applications)