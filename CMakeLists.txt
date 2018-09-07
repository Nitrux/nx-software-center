project(nx-software-center)
set(PROJECT_VERSION 2.3)
cmake_minimum_required(VERSION 3.3)

cmake_policy(SET CMP0037 NEW)
cmake_policy(SET CMP0063 NEW)

# Enable C++14 support
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

# Enable exceptions
add_compile_options("-fexceptions")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


include(FeatureSummary)
include(ExternalProject)

include(cmake/libappimage.cmake)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

install(FILES "org.nxos.softwarecenter.desktop" DESTINATION share/applications)
include(cmake/cpack_deb.cmake)