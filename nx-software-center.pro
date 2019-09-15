QT += qml
QT += quick
QT += sql
QT += widgets
QT += quickcontrols2

TARGET = nx-software-center
TEMPLATE = app

CONFIG += ordered
CONFIG += c++17
QMAKE_LINK += -nostdlib++

DESTDIR = $$OUT_PWD/

linux:unix:!android {
} else:android {

    message(Building helpers for Android)
    QT += androidextras webview
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

SOURCES += \
    src/main.cpp \
    src/models/appsmodel.cpp \
#    $$PWD/src/models/storemodel.cpp \

HEADERS += \
    src/utils/nx.h \
    src/models/appsmodel.h \
#    $$PWD/src/models/storemodel.h \


RESOURCES += \
    src/ui/qml.qrc \
    assets/nx-assets.qrc \

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/3rdparty/mauikit/src/android
}

DISTFILES += \
    3rdparty/mauikit/src/android/AndroidManifest.xml \
    3rdparty/mauikit/src/android/build.gradle \
    3rdparty/mauikit/src/android/build.gradle \
    3rdparty/mauikit/src/android/gradle/wrapper/gradle-wrapper.jar \
    3rdparty/mauikit/src/android/gradle/wrapper/gradle-wrapper.jar \
    3rdparty/mauikit/src/android/gradle/wrapper/gradle-wrapper.properties \
    3rdparty/mauikit/src/android/gradle/wrapper/gradle-wrapper.properties \
    3rdparty/mauikit/src/android/gradlew \
    3rdparty/mauikit/src/android/gradlew \
    3rdparty/mauikit/src/android/gradlew.bat \
    3rdparty/mauikit/src/android/gradlew.bat \
    3rdparty/mauikit/src/android/res/values/libs.xml \
    3rdparty/mauikit/src/android/res/values/libs.xml
