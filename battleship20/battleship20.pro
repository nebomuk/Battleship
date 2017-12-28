TEMPLATE = app
TARGET = battleship
CONFIG += warn_on
DESTDIR = bin
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
	src/counteritem.h \
	src/gamestate.h \
	src/control.h \
	src/emitter.h \
	src/ai.h \
	src/mainmenu.h \
    src/jsproxy.h
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
FORMS = ui/mainmenu.ui
QT += svg \
	script \
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
	src/counteritem.cpp \
	src/mainmenu.cpp \
    src/jsproxy.cpp
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
