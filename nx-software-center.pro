QT += qml
QT += quick
QT += sql

CONFIG += c++17

linux:unix:!android {
} else:android {

    message(Building helpers for Android)
    QT *= androidextras
        QMAKE_LINK += -nostdlib++
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android_files

    DEFINES *= \
#        COMPONENT_EDITOR \
        COMPONENT_ACCOUNTS \
        COMPONENT_FM \
#        COMPONENT_TERMINAL \
        COMPONENT_TAGGING \
#        COMPONENT_SYNCING \
        MAUIKIT_STYLE \
        ANDROID_OPENSSL

    include($$PWD/3rdparty/kirigami/kirigami.pri)
    include($$PWD/3rdparty/mauikit/mauikit.pri)

    DEFINES += STATIC_KIRIGAMI

} else {
    message("Unknown configuration")
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=  $$PWD/src/stores

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/models/appsmodel.cpp \
    $$PWD/src/models/storemodel.cpp \
    $$PWD/src/models/categoriesmodel.cpp \
    $$PWD/src/stores/opendesktopstore.cpp \
    $$PWD/src/stores/appimagehubstore.cpp \
    $$PWD/src/stores/ResponseDTO/category.cpp


HEADERS += \
    $$PWD/src/utils/nx.h \
    $$PWD/src/models/appsmodel.h \
    $$PWD/src/models/storemodel.h \
    $$PWD/src/models/categoriesmodel.h \
    $$PWD/src/stores/opendesktopstore.h \
    $$PWD/src/stores/appimagehubstore.h \
    $$PWD/src/stores/store.h \
    $$PWD/src/stores/ResponseDTO/category.h \
    $$PWD/src/stores/ResponseDTO/categoryresponsedto.h \
    $$PWD/src/stores/ResponseDTO/application.h \
    $$PWD/src/stores/ResponseDTO/applicationresponsedto.h


RESOURCES += \
    $$PWD/src/qml.qrc \
     $$PWD/assets/nx-assets.qrc \

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
