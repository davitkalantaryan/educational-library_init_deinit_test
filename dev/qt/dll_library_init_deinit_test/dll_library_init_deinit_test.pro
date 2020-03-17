
QT -= gui
QT -= core
QT -= widgets
CONFIG -= qt

QMAKE_LFLAGS = -Wl,-E -pie -shared

SOURCES += \
    $${PWD}/../../../entry_library_init_deinit_test.cxx
