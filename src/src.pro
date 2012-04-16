include( ../config.pri )

TEMPLATE = app
TARGET   = descend

CONFIG  += qt
QT      += opengl

HEADERS += mainwindow.h \
           scenewidget.h \
           surfacetessellator.h

SOURCES += main.cpp \
           mainwindow.cpp \
           scenewidget.cpp \
           surfacetessellator.cpp

FORMS   += mainwindow.ui

RESOURCES += \
           shaders/shaders.qrc

include( misc/misc.pri )
include( scene/scene.pri )

INCLUDEPATH += .

PRECOMPILED_HEADER = precompiled.h

win32 {
    RC_FILE = descend.rc
}

win32-msvc* {
    QMAKE_CXXFLAGS += -Fd\$(IntDir)
    CONFIG -= flat
}

!win32 | build_pass {
    MOC_DIR = ../tmp
    RCC_DIR = ../tmp
    UI_DIR = ../tmp
    CONFIG( debug, debug|release ) {
        OBJECTS_DIR = ../tmp/debug
        DESTDIR = ../debug
    } else {
        OBJECTS_DIR = ../tmp/release
        DESTDIR = ../release
    }
}

target.path = $${DESTINATION}$$PREFIX/bin
INSTALLS += target
