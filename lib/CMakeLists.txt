add_subdirectory(appimage-metadata-extractor)

add_library(appimage-info ALIAS app_core)
SET(APPIMAGE_INFO_INCLUDE_DIRECTORIES "appimage-metadata-extractor/src/entities/")
