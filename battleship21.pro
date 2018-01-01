TEMPLATE = app
TARGET = battleship
CONFIG += warn_on
android {
    QT += androidextras
    SOURCES += src/androidhelper.cpp
    HEADERS += src/androidhelper.h
}

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
        qml

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
    script.qrc
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
