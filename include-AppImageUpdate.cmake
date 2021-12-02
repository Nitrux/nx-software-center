find_library(APPIMAGEUPDATE_IMPORTED_LOCATION NAMES appimageupdate NO_CMAKE_SYSTEM_PATH)

if ("${APPIMAGEUPDATE_IMPORTED_LOCATION}" STREQUAL "APPIMAGEUPDATE_IMPORTED_LOCATION-NOTFOUND")
    message(STATUS "Importing AppImageUpdate from sources")
    include(ExternalProject)

    set(AppImageUpdate_ROOT "${CMAKE_CURRENT_BINARY_DIR}/AppImageUpdate")
    externalproject_add(
        AppImageUpdate
        GIT_REPOSITORY "https://github.com/AppImage/AppImageUpdate"
        GIT_TAG "main"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${AppImageUpdate_ROOT}"
        CONFIGURE_HANDLED_BY_BUILD ON
        BUILD_BYPRODUCTS "${AppImageUpdate_ROOT}/lib/libappimageupdate.so"
    )

    file(MAKE_DIRECTORY ${AppImageUpdate_ROOT}/include)  # avoid race condition

    # make AppImageUpdate targets available as imported targets
    add_library(AppImageUpdate::libappimageupdate SHARED IMPORTED GLOBAL)
    set_target_properties(
        AppImageUpdate::libappimageupdate PROPERTIES
        IMPORTED_LOCATION ${AppImageUpdate_ROOT}/lib/libappimageupdate.so
        INTERFACE_INCLUDE_DIRECTORIES ${AppImageUpdate_ROOT}/include
        INTERFACE_LINK_DIRECTORIES ${AppImageUpdate_ROOT}/lib
    )

    add_dependencies(AppImageUpdate::libappimageupdate AppImageUpdate)
else ()
    message(STATUS "Importing AppImageUpdate form system")
    add_library(AppImageUpdate::libappimageupdate SHARED IMPORTED GLOBAL)
    set_target_properties(
        AppImageUpdate::libappimageupdate PROPERTIES
        IMPORTED_LOCATION ${APPIMAGEUPDATE_IMPORTED_LOCATION}
    )
endif ()

