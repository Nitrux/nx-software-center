add_subdirectory(appimage-metadata-extractor)

add_library(appimage-info SHARED $<TARGET_OBJECTS:app_core>)
target_link_libraries(appimage-info ${APPIMAGEKIT_LIBRARIES} Qt5::Core)
target_include_directories(appimage-info INTERFACE appimage-metadata-extractor/src/entities/)