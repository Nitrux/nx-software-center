message(STATUS "Importing AppImageUpdate from sources")
include(ExternalProject)

set(AppImageUpdate_ROOT "${CMAKE_CURRENT_BINARY_DIR}/AppImageUpdate")
externalproject_add(
    AppImageUpdate
    GIT_REPOSITORY "https://github.com/AppImage/AppImageUpdate"
    GIT_TAG "main"
    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${AppImageUpdate_ROOT}"
    CONFIGURE_HANDLED_BY_BUILD ON
    UPDATE_DISCONNECTED ON
    BUILD_BYPRODUCTS "${AppImageUpdate_ROOT}/lib/libappimageupdate.so"
    INSTALL_COMMAND make DESTDIR= install
)

file(MAKE_DIRECTORY ${AppImageUpdate_ROOT}/include)  # avoid race condition

# make AppImageUpdate targets available as imported targets
add_library(libappimageupdate SHARED IMPORTED GLOBAL)
set_target_properties(
    libappimageupdate PROPERTIES
    IMPORTED_LOCATION ${AppImageUpdate_ROOT}/lib/libappimageupdate.so
    INTERFACE_INCLUDE_DIRECTORIES ${AppImageUpdate_ROOT}/include
    INTERFACE_LINK_DIRECTORIES ${AppImageUpdate_ROOT}/lib
)

add_dependencies(libappimageupdate AppImageUpdate)
