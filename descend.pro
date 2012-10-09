include( config.pri )

TEMPLATE = subdirs
SUBDIRS  = src

templates.files = templates/*.dscn templates/templates.ini
win32 {
    templates.path = $$PREFIX/templates
} else {
    templates.path = $${DESTINATION}$$PREFIX/share/descend/templates
}
INSTALLS += templates

QMAKE_DISTCLEAN += config.pri
