add_executable(
    test_storemanager_service

    teststoremanagerservice.cpp

    ../../../src/stores/ResponseDTO/pling_store_application_data.cpp
    ../../../src/stores/ResponseDTO/category.cpp
    ../../../src/stores/ResponseDTO/categoryresponsedto.h
    ../../../src/stores/ResponseDTO/applicationresponsedto.h
    ../../../src/stores/ResponseDTO/apprepo/apprepogroupresponsedto.cpp
    ../../../src/stores/ResponseDTO/apprepo/apprepopackageresponsedto.cpp
    ../../../src/stores/ResponseDTO/apprepo/apprepoversiondto.cpp
    ../../../src/stores/store.h
    ../../../src/stores/opendesktopstore.cpp
    ../../../src/stores/apprepostore.cpp
    ../../../src/stores/appimagehubstore.cpp
    ../../../src/services/storemanager/storemanager.h
    ../../../src/services/storemanager/apprepostoremanager.cpp
    ../../../src/services/storemanager/appimagehubstoremanager.cpp
    ../../../src/services/storemanager/storemanagerservice.cpp
)

target_link_libraries(test_storemanager_service PRIVATE Qt5::Widgets Qt5::Test Qt5::Network)
target_include_directories(test_storemanager_service PRIVATE ../../../src)
add_test(NAME testStoreManagerService COMMAND test_storemanager_service)