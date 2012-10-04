contains( QT_CONFIG, system-zlib ) {
    if( unix|win32-g++* ): LIBS += -lz
    else: LIBS += zdll.lib
} else {
    INCLUDEPATH += $$PWD
}
