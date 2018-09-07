message(STATUS "Downloading and building GTest")

if (NOT TARGET gtest)
    ExternalProject_Add(gtest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.8.0
            UPDATE_COMMAND ""  # make sure CMake won't try to updateRepository updates unnecessarily and hence rebuild the dependency every time
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> <SOURCE_DIR>/googletest
            )

    ExternalProject_Get_Property(gtest SOURCE_DIR)
    ExternalProject_Get_Property(gtest INSTALL_DIR)
    set(gtest_SOURCE_DIR "${SOURCE_DIR}")
    set(gtest_INSTALL_DIR "${INSTALL_DIR}")
    mark_as_advanced(gtest_SOURCE_DIR gtest_INSTALL_DIR)

    set(gtest_INCLUDE_DIRS "${gtest_INSTALL_DIR}/include/")
    set(gtest_LIBRARIES_DIR "${gtest_INSTALL_DIR}/lib")
    set(gtest_LIBRARIES "${gtest_LIBRARIES_DIR}/libgtest.a" "${gtest_LIBRARIES_DIR}/libgtest_main.a")
endif (NOT TARGET gtest)