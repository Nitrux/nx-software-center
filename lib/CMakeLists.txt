# needed to extract AppImage information
add_subdirectory(AppImageInfo EXCLUDE_FROM_ALL)
set(NLOHMANN_JSON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/AppImageInfo/thirdparty/nlohmann_json/include" PARENT_SCOPE)
