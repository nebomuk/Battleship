TEMPLATE = app
TARGET = battleship
CONFIG += warn_on


HEADERS = src/vehicle.h \
	src/advancetimer.h \
	src/ballisticprojectile.h \
	src/globalvariables.h \
	src/signum.h \
	src/uprisetorpedo.h \
	src/movingitem.h \
	src/centerrectinrect.h \
	src/svgcache.h \
	src/graphicsview.h \
	src/animatedsvgitem.h \
	src/graphicsengine.h \
	src/gamestate.h \
	src/mainmenu.h \
    src/jsproxy.h \
    src/graphicssoftbutton.h

FORMS = ui/mainmenu.ui
QT += svg \
        opengl \
        qml quick \
        widgets

greaterThan(QT_MAJOR_VERSION, 5) {
    QT += svgwidgets
}

# ModelScene.qml only loaded on non mobile os
!android&!ios{

QT += 3dcore 3drender 3dinput 3dquick 3dlogic  3dquickextras
}

DEFINES += QT_DEPRECATED_WARNINGS

QTPLUGIN += qsvg

SOURCES = src/vehicle.cpp \
	src/ballisticprojectile.cpp \
	src/main.cpp \
	src/uprisetorpedo.cpp \
	src/animatedsvgitem.cpp \
	src/movingitem.cpp \
	src/svgcache.cpp \
	src/graphicsview.cpp \
	src/graphicsengine.cpp \
	src/mainmenu.cpp \
    src/jsproxy.cpp \
    src/graphicssoftbutton.cpp
RESOURCES = images2.qrc \
	explosion_images1.qrc \
	cloud_images.qrc \
	translations.qrc \
    script.qrc \
    menu_images.qrc \
    models.qrc \
    qml.qrc
OTHER_FILES += src/mainloop.js \
	src/emitter.js \
	src/control.js \
	src/ai.js

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
