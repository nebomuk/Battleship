TEMPLATE = app
TARGET = battleship
CONFIG += warn_on
android {
    QT += androidextras
    SOURCES += src/androidhelper.cpp #my android specific cpp file
    HEADERS += src/androidhelper.h   #my android specific header file
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
    src/androidhelper.h \
    src/graphicssoftbutton.h
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
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
    src/androidhelper.cpp \
    src/graphicssoftbutton.cpp
RESOURCES = ../images2.qrc \
	../hitpointsBar_images.qrc \
	../explosion_images1.qrc \
	../digits_images.qrc \
	../cloud_images.qrc \
	../translations.qrc \
    ../script.qrc
OTHER_FILES += src/mainloop.js \
	src/emitter.js \
	src/control.js \
	src/ai.js
