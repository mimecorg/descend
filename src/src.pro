include( ../config.pri )

TEMPLATE = app
TARGET   = descend

CONFIG  += qt
QT      += opengl xml

HEADERS += application.h \
           mainwindow.h \
           scenewidget.h

SOURCES += application.cpp \
           main.cpp \
           mainwindow.cpp \
           scenewidget.cpp

FORMS   += mainwindow.ui

RESOURCES += \
           shaders/shaders.qrc

include( misc/misc.pri )
include( scene/scene.pri )
include( utils/utils.pri )
include( xmlui/xmlui.pri )

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
