
QT -= gui
QT -= core
QT -= widgets
CONFIG -= qt

LIBS += -ldl
LIBS += -pthread

SOURCES += \
    $${PWD}/../../../main_library_init_deinit_test.cpp
