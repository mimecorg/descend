include( ../config.pri )

TEMPLATE = app
TARGET   = descend

CONFIG  += qt
QT      += opengl xml

HEADERS += application.h \
           mainwindow.h

SOURCES += application.cpp \
           main.cpp \
           mainwindow.cpp

FORMS   += mainwindow.ui

RESOURCES += \
           icons/icons.qrc \
           resources/resources.qrc \
           shaders/shaders.qrc

include( adapters/adapters.pri )
include( dialogs/dialogs.pri )
include( misc/misc.pri )
include( project/project.pri )
include( scene/scene.pri )
include( utils/utils.pri )
include( widgets/widgets.pri )
include( xmlui/xmlui.pri )
include( zlib/zlib.pri )

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
