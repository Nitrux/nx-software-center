find_package(libappimage QUIET)

if (NOT TARGET libappimage)
    message(STATUS "Downloading and building libappimage")
    include(ExternalProject)

    ExternalProject_Add(libappimage_project
            GIT_REPOSITORY https://github.com/AppImage/libappimage.git
            GIT_TAG master
            GIT_SUBMODULES ""
            GIT_SHALLOW 1
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} <SOURCE_DIR>
            BUILD_COMMAND make
            INSTALL_COMMAND make install DESTDIR=<INSTALL_DIR>)

    ExternalProject_Get_Property(libappimage_project INSTALL_DIR)
    set(LIBAPPIMAGE_PROJECT_INSTALL_DIR ${INSTALL_DIR})
    set(LIBAPPIMAGE_PROJECT_INSTALL_LIB_DIR ${LIBAPPIMAGE_PROJECT_INSTALL_DIR}${CMAKE_INSTALL_PREFIX}/lib/)
    set(LIBAPPIMAGE_PROJECT_INSTALL_INCLUDE_DIR ${LIBAPPIMAGE_PROJECT_INSTALL_DIR}${CMAKE_INSTALL_PREFIX}/include/)
    add_library(libappimage SHARED IMPORTED GLOBAL)

    # Hack to make INTERFACE_INCLUDE_DIRECTORIES to be propagated
    file(MAKE_DIRECTORY ${LIBAPPIMAGE_PROJECT_INSTALL_INCLUDE_DIR})

    set_target_properties(libappimage PROPERTIES
            IMPORTED_LOCATION ${LIBAPPIMAGE_PROJECT_INSTALL_LIB_DIR}/libappimage.so
            INTERFACE_INCLUDE_DIRECTORIES ${LIBAPPIMAGE_PROJECT_INSTALL_INCLUDE_DIR})

    add_dependencies(libappimage libappimage_project)
    install(FILES "${LIBAPPIMAGE_PROJECT_INSTALL_DIR}${CMAKE_INSTALL_PREFIX}/lib/libappimage.so" DESTINATION lib COMPONENT desktop)
endif (NOT TARGET libappimage)